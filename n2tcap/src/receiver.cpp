/// Martin Slezák (xsleza26)

#include <stdexcept>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <ctime>
#include <iomanip>

#include "receiver.hpp"

constexpr int BUFFER_SIZE = 1444;

Receiver::Receiver(char *hostname, char *port) {
    addrinfo *res;
    if (getaddrinfo(hostname, port, nullptr, &res) != 0) {
        throw std::runtime_error("resolving hostname");
    }

    addr = std::vector<char>(res->ai_addrlen);
    auto addr_src = reinterpret_cast<const char*>(res->ai_addr);
    std::copy(addr_src, addr_src + res->ai_addrlen, addr.data());

    socket_fd = socket(res->ai_family, SOCK_DGRAM, 0);
    freeaddrinfo(res);
}

void Receiver::start() {
    bind_socket();

    char buffer[BUFFER_SIZE];
    sockaddr_in caddr{};
    socklen_t caddr_len = sizeof(caddr);

    while (true) {
        ssize_t recv_len = recvfrom(
            socket_fd, buffer, BUFFER_SIZE, 0,
            reinterpret_cast<sockaddr*>(&caddr), &caddr_len
        );

        if (recv_len < 0) {
            std::cerr << "Error receiving data\n";
            continue;
        }

        parse_packet(buffer);
    }
}

void Receiver::parse_packet(char *buffer) {
    auto header = reinterpret_cast<NetflowV5Header*>(buffer);
    print_header(header);

    std::cout << "Flows:" << std::endl;
    buffer += sizeof(NetflowV5Header);
    for (int i = 0; i < header->count; ++i) {
        auto flow = reinterpret_cast<NetflowV5Flow*>(buffer);
        print_flow(flow);
        buffer += sizeof(NetflowV5Flow);
    }
}

void Receiver::print_header(NetflowV5Header *header) {
    header->endian_unprep();

    using namespace std::chrono;
    auto dur = duration_cast<system_clock::duration>(
        seconds(header->unix_secs) + nanoseconds(header->unix_nsecs)
    );
    auto time = system_clock::time_point(dur);

    auto samp = ntohs(header->sampling_interval);
    auto samp_mode = (samp >> 14) & 0x03;
    auto samp_int = samp & 0x3FFF;

    std::cout << "Header:\n"
        << "  version   : " << ntohs(header->version) << "\n"
        << "  count     : " << header->count << "\n"
        << "  sys_uptime: " << header->sys_uptime << "ms\n"
        << "  time      : ";
    print_time(time);
    std::cout << "\n"
        << "  flow seq. : " << header->flow_sequence << "\n"
        << "  eng. type : " << int(header->engine_type) << "\n"
        << "  eng. id   : " << int(header->engine_id) << "\n"
        << "  sampling  : " << samp_mode << ":" << samp_int << "\n";
}

void Receiver::print_flow(NetflowV5Flow *flow) {
    flow->to_be();
    std::cout
        << "------------------------------\n"
        << "src     : ";
    print_ip(flow->src_addr);
    std::cout << ":" << flow->src_port << "\n"
        << "dst     : ";
    print_ip(flow->dst_addr);
    std::cout << ":" << flow->dst_port << "\n"
        << "nexthop : ";
    print_ip(flow->nexthop);
    std::cout << "\n"
        << "input   : " << flow->input << "\n"
        << "output  : " << flow->output << "\n"
        << "pkts    : " << flow->d_pkts << "\n"
        << "octets  : " << flow->d_octets << " bytes\n"
        << "first   : " << flow->first << "\n"
        << "last    : " << flow->last << "\n"
        << "flags   : " << int(flow->tcp_flags) << "\n"
        << "prot    : " << int(flow->prot) << "\n"
        << "tos     : " << int(flow->tos) << "\n"
        << "src as  : " << flow->src_as << "\n"
        << "dst as  : " << flow->dst_as << "\n"
        << "src mask: " << int(flow->src_mask) << "\n"
        << "dst mask: " << int(flow->dst_mask) << "\n";
}

void Receiver::print_ip(uint32_t ip) {
    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &ip, ip_str, INET_ADDRSTRLEN) != nullptr) {
        std::cout << ip_str;
    } else {
        std::cout << "-";
    }
}

void Receiver::print_time(std::chrono::system_clock::time_point time) {
    using namespace std::chrono;
    auto tt = system_clock::to_time_t(time);
    auto tm = *std::gmtime(&tt);
    auto time_str = std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    auto dur = time.time_since_epoch();
    auto time_ms = duration_cast<milliseconds>(dur);
    auto time_ns = duration_cast<nanoseconds>(dur);
    auto ns = (time_ns - time_ms).count();
    std::cout << time_str << "." << std::setw(9) << std::setfill('0') << ns;
}

void Receiver::bind_socket() {
    sockaddr *sock_addr = reinterpret_cast<sockaddr*>(addr.data());
    if (bind(socket_fd, sock_addr, addr.size()) < 0) {
        close_socket();
        throw std::runtime_error("binding socket");
    }
}

void Receiver::close_socket() {
    close(socket_fd);
    socket_fd = -1;
}

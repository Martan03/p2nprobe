/// Martin Slezák (xsleza26)

#include "parser.hpp"
#include "udp_client.hpp"

#include <stdexcept>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#define ETH_ADDR_LEN 6
#define ETH_HEADER_LEN 14

Parser::Parser(std::string file): flows() {
    pcap = pcap_open_offline(file.c_str(), err_buf);
    if (pcap == nullptr) {
        throw std::runtime_error("Could not open PCAP file");
    }

    gettimeofday(&uptime, nullptr);
}

void Parser::parse() {
    pcap_pkthdr *header;
    const u_char *packet;

    while (int ret = pcap_next_ex(pcap, &header, &packet) >= 0) {
        if (ret == 0)
            continue;

        process_packet(header, packet);
    }

    pcap_close(pcap);
}

void Parser::process_packet(pcap_pkthdr *header, const u_char *packet) {
    auto eth = reinterpret_cast<const ether_header*>(packet);
    auto ip_header = reinterpret_cast<const ip*>(packet + ETH_HEADER_LEN);
    int ip_header_len = ip_header->ip_hl * 4;
    if (ip_header_len < 20) {
        // std::cout <<
        //     "Invalid IP header length: " << ip_header_len << " bytes"
        // << std::endl;
        return;
    }

    if (ip_header->ip_p != IPPROTO_TCP) {
        return;
    }

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);

    auto tcp_header = reinterpret_cast<const tcphdr*>(
        packet + ETH_HEADER_LEN + ip_header_len
    );

    FlowKey key {
        .src_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_src),
        .dst_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_dst),
        .src_port = tcp_header->source,
        .dst_port = tcp_header->dest,
        .tos = ip_header->ip_tos,
    };
    if (flows.contains(key)) {
        // TODO: edit all values
        auto cur = flows[key];
        cur.d_pkts++;
        cur.last = get_timestamp(header->ts);
        flows[key] = cur;
    } else {
        // TODO: insert all values
        NetflowV5Flow flow(key, get_timestamp(header->ts));
        flows[key] = flow;
    }
}

uint32_t Parser::get_timestamp(timeval time) {
    auto secs = uptime.tv_sec - time.tv_sec;
    auto usecs = uptime.tv_usec - time.tv_usec;
    return secs * 1000.0 + usecs / 1000.0;
}

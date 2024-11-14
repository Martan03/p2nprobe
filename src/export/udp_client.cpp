/// Martin Slezák (xsleza26)

#include "udp_client.hpp"

#include <iostream>
#include <netdb.h>
#include <cstring>
#include <string>

UdpClient::UdpClient(Args args):
    UdpClient(args.host.value(), args.port.value())
{}

// TODO: throw exception instead of printing the error
UdpClient::UdpClient(std::string hostname, std::uint16_t port) {
    addrinfo *res;

    int status = getaddrinfo(
        hostname.c_str(),
        std::to_string(port).c_str(),
        nullptr,
        &res
    );
    if (status != 0) {
        std::cerr << "Error resolving the hostname" << std::endl;
        return;
    }

    addr = std::vector<char>(res->ai_addrlen);
    auto addr_src = reinterpret_cast<const char*>(res->ai_addr);
    std::copy(addr_src, addr_src + res->ai_addrlen, addr.data());

    socket_fd = socket(res->ai_family, SOCK_DGRAM, 0);
    freeaddrinfo(res);

    if (socket_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }
}

void UdpClient::send(std::vector<char> data) {
    sockaddr *sock_addr = reinterpret_cast<sockaddr*>(addr.data());
    ssize_t sent =
        sendto(socket_fd, data.data(), data.size(), 0, sock_addr, addr.size());
    if (sent < 0) {
        perror("");
        std::cerr << "Error sending data" << std::endl;
    }
}

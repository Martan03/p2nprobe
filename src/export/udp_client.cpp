/// Martin Slezák (xsleza26)

#include "udp_client.hpp"

#include <iostream>
#include <netdb.h>
#include <cstring>
#include <string>

UdpClient::UdpClient(Args args):
    UdpClient(args.host.value(), args.port.value())
{}

UdpClient::UdpClient(std::string hostname, std::uint16_t port) {
    addrinfo *res;

    auto sport = std::to_string(port).c_str();
    int status = getaddrinfo(hostname.c_str(), sport, nullptr, &res);
    if (status != 0) {
        throw std::runtime_error("Error resolving the hostname");
    }

    addr = std::vector<char>(res->ai_addrlen);
    auto addr_src = reinterpret_cast<const char*>(res->ai_addr);
    std::copy(addr_src, addr_src + res->ai_addrlen, addr.data());

    socket_fd = socket(res->ai_family, SOCK_DGRAM, 0);
    freeaddrinfo(res);

    if (socket_fd < 0) {
        throw std::runtime_error("Error creating socket");
    }
}

void UdpClient::send(std::vector<char> data) {
    sockaddr *sock_addr = reinterpret_cast<sockaddr*>(addr.data());
    ssize_t sent =
        sendto(socket_fd, data.data(), data.size(), 0, sock_addr, addr.size());
    if (sent < 0) {
        throw std::runtime_error("Error sending data");
    }
}

/// Martin Slezák (xsleza26)

#include "udp_client.hpp"

#include <iostream>
#include <netdb.h>
#include <cstring>
#include <vector>

UdpClient::UdpClient(Args args) {
    UdpClient(args.host.value(), args.port.value());
}

// TODO: throw exception instead of printing the error
UdpClient::UdpClient(std::string hostname, std::uint16_t port):
    addr()
{
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    addrinfo hints, *res;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "Error resolving the hostname" << std::endl;
        close(socket_fd);
        socket_fd = -1;
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = (reinterpret_cast<sockaddr_in*>(res->ai_addr))->sin_addr;
    freeaddrinfo(res);
}

void UdpClient::send(std::span<NetflowV5Flow> flows) {
    // TODO: flow header
    NetflowV5Header header{};

    auto flows_size = flows.size() * sizeof(NetflowV5Flow);
    std::vector<uint8_t> buffer(sizeof(NetflowV5Header) + flows_size);
    memcpy(buffer.data(), &header, sizeof(NetflowV5Header));
    memcpy(buffer.data() + sizeof(NetflowV5Header), flows.data(), flows_size);

    sockaddr *addr = reinterpret_cast<sockaddr*>(&addr);
    ssize_t sent =
        sendto(socket_fd, buffer.data(), buffer.size(), 0, addr, sizeof(addr));
    if (sent < 0) {
        std::cerr << "Error sending the flow" << std::endl;
        return;
    }
}

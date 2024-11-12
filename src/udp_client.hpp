/// Martin Slezák (xsleza26)

#pragma once

#include "netflow/header.hpp"
#include "netflow/flow.hpp"

#include <cstdint>
#include <string>
#include <span>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class UdpClient {
    public:
        /// @brief Creates new UDP client
        /// @param hostname hostname of the server to connect to
        /// @param port port of the server to connect to
        UdpClient(std::string hostname, std::uint16_t port);

        /// @brief Closes the socket on destruction
        ~UdpClient() {
            if (socket_fd != -1) {
                close(socket_fd);
                socket_fd = -1;
            }
        }

        /// @brief Exports given flows
        /// @param flows flows to be exported
        void send(std::span<NetflowV5Flow> flows);

    private:
        int socket_fd = -1;
        sockaddr_in addr;
};

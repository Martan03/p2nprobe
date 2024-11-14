/// Martin Slezák (xsleza26)

#pragma once

#include "../args.hpp"
#include "../netflow/header.hpp"
#include "../netflow/netflow.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

class UdpClient {
    public:
        /// @brief Creates new UDP client
        /// @param args arguments containing the hostname and port
        UdpClient(Args args);

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

        /// @brief Sends data to the machine given by hostname and port
        /// @param data data to be send
        void send(std::vector<char> data);

    private:
        int socket_fd = -1;
        std::vector<char> addr;
};

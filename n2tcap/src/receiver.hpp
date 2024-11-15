/// Martin Slezák (xsleza26)

#pragma once

#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <chrono>

#include "../../src/netflow/header.hpp"
#include "../../src/netflow/netflow.hpp"

class Receiver {
    public:
        /// @brief Creates new receiver
        /// @param hostname hostname to receive packets from
        /// @param port port to receive packets from
        Receiver(char *hostname, char *port);

        /// @brief Closes the socket on destruction
        ~Receiver() {
            if (socket_fd != -1) {
                close_socket();
            }
        }

        /// @brief Starts receiving packets and printing the Netflow data
        void start();

    private:
        int socket_fd = -1;
        std::vector<char> addr;

        /// @brief Parses the received data and prints the netflow data
        /// @param buffer received data
        void parse_packet(char *buffer);

        /// @brief Prints the NetflowV5Header details
        /// @param header captured NetflowV5Header
        void print_header(NetflowV5Header *header);

        /// @brief Prints the NetflowV5Flow details
        /// @param flow captured NetflowV5Flow
        void print_flow(NetflowV5Flow *flow);

        /// @brief Prints IP address
        /// @param ip address to be printed
        void print_ip(uint32_t ip);

        /// @brief Prints the time in the UTC format
        /// @param time time to be printed
        void print_time(std::chrono::system_clock::time_point time);

        /// @brief Binds the socket to the address
        void bind_socket();

        /// @brief Closes the socket
        void close_socket();
};

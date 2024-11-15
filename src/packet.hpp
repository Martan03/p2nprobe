/// Martin Slezák (xsleza26)

#pragma once

#include <cstdint>
#include <chrono>

struct Packet {
    /// @brief Source IP address
    uint32_t src_addr;
    /// @brief Destination IP address
    uint32_t dst_addr;
    /// @brief TCP/UDP source port number or equivalent
    uint16_t src_port;
    /// @brief TCP/UDP destination port number or equivalent
    uint16_t dst_port;
    /// @brief IP type of service;
    uint8_t tos;
    /// @brief Cumulative OR of TCP flags
    uint8_t tcp_flags = 0;
    /// @brief Size of packet in L3 in bytes
    uint32_t size;
    /// @brief Time packet was sent
    std::chrono::system_clock::time_point time;

    Packet() = default;
};

/// Martin Slezák (xsleza26)

#pragma once

#include <cstdint>
#include <chrono>

#include "flow_key.hpp"

struct Flow {
    /// @brief Source IP address
    uint32_t src_addr;
    /// @brief Destination IP address
    uint32_t dst_addr;
    /// @brief Packets in the flow
    uint32_t d_pkts = 1;
    /// @brief Total number of Layer 3 bytes in packets of the flow
    uint32_t d_octets = 0;
    /// @brief SysUptime at start of flow
    std::chrono::system_clock::time_point first;
    /// @brief SysUptime when the last packet of the flow was received
    std::chrono::system_clock::time_point last;
    /// @brief TCP/UDP source port number or equivalent
    uint16_t src_port;
    /// @brief TCP/UDP destination port number or equivalent
    uint16_t dst_port;
    /// @brief Cumulative OR of TCP flags
    uint8_t tcp_flags = 0;
    /// @brief IP type of service;
    uint8_t tos;

    Flow() = default;

    Flow(FlowKey key, std::chrono::system_clock::time_point uptime):
        src_addr(key.src_addr),
        dst_addr(key.dst_addr),
        src_port(key.src_port),
        dst_port(key.dst_port),
        tos(key.tos),
        first(uptime),
        last(uptime)
    {}
};

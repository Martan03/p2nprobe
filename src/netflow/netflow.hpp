/// Martin Slezák (xsleza26)
///
/// Flow details were found on the link below
/// https://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html#wp1006186

#pragma once

#include <cstdint>
#include <chrono>
#include <netinet/in.h>

#include "flow_key.hpp"
#include "flow.hpp"

struct __attribute__ ((packed)) NetflowV5Flow {
    /// @brief Source IP address
    uint32_t src_addr;
    /// @brief Destination IP address
    uint32_t dst_addr;
    /// @brief IP address of next hop router
    uint32_t nexthop = 0;
    /// @brief SNMP index of input interface
    uint16_t input = 0;
    /// @brief SNMP index of output interface
    uint16_t output = 0;
    /// @brief Packets in the flow
    uint32_t d_pkts;
    /// @brief Total number of Layer 3 bytes in packets of the flow
    uint32_t d_octets = 0;
    /// @brief SysUptime at start of flow (msecs)
    uint32_t first;
    /// @brief SysUptime when the last packet of the flow was received (msecs)
    uint32_t last;
    /// @brief TCP/UDP source port number or equivalent
    uint16_t src_port;
    /// @brief TCP/UDP destination port number or equivalent
    uint16_t dst_port;
    /// @brief Unused (zero) bytes
    uint8_t pad1 = 0;
    /// @brief Cumulative OR of TCP flags
    uint8_t tcp_flags = 0;
    /// @brief IP protocol type
    uint8_t prot = 6;
    /// @brief IP type of service;
    uint8_t tos;
    /// @brief Autonomous system number of the source (origin or peer)
    uint16_t src_as = 0;
    /// @brief Autonomous system number of the destination (origin or peer)
    uint16_t dst_as = 0;
    /// @brief Source address prefix mask bits
    uint8_t src_mask = 0;
    /// @brief Destination address prefix mask bits
    uint8_t dst_mask = 0;
    /// @brief Unused (zero) bytes
    uint16_t pad2 = 0;

    NetflowV5Flow() = default;

    NetflowV5Flow(Flow flow, std::chrono::system_clock::time_point uptime):
        src_addr(flow.src_addr),
        dst_addr(flow.dst_addr),
        d_pkts(htonl(flow.d_pkts)),
        d_octets(htonl(flow.d_octets)),
        src_port(flow.src_port),
        dst_port(flow.dst_port),
        tcp_flags(flow.tcp_flags),
        tos(flow.tos)
    {
        using namespace std::chrono;
        auto fms = duration_cast<milliseconds>(flow.first - uptime);
        first = htonl(static_cast<uint32_t>(fms.count()));

        auto lms = duration_cast<milliseconds>(flow.last - uptime);
        last = htonl(static_cast<uint32_t>(lms.count()));
    }
};

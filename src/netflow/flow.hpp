/// Martin Slezák (xsleza26)
///
/// Flow details were found on the link below
/// https://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html#wp1006186

#pragma once

#include <stdint.h>

#include "flow_key.hpp"

struct NetflowV5Flow {
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
    uint32_t d_pkts = 1;
    /// @brief Total number of Layer 3 bytes in packets of the flow
    uint32_t d_octets = 0;
    /// @brief SysUptime at start of flow
    uint32_t first;
    /// @brief SysUptime when the last packet of the flow was received
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

    NetflowV5Flow(FlowKey key, uint32_t uptime):
        src_addr(key.src_addr),
        dst_addr(key.dst_addr),
        src_port(key.src_port),
        dst_port(key.dst_port),
        tos(key.tos),
        first(uptime),
        last(uptime)
    {}
};

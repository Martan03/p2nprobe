/// Martin Slezák (xsleza26)

#pragma once

#include <stdint.h>

class NetflowV5Record {
    public:
        /// @brief Source IP address
        uint32_t scr_addr;
        /// @brief Destination IP address
        uint32_t dst_addr;
        /// @brief IP address of next hop router
        uint32_t nexthop;
        /// @brief SNMP index of input interface
        uint16_t input;
        /// @brief SNMP index of output interface
        uint16_t output;
        /// @brief Packets in the flow
        uint32_t d_pkts;
        /// @brief Total number of Layer 3 bytes in packets of the flow
        uint32_t d_octets;
        /// @brief SysUptime at start of flow
        uint32_t first;
        /// @brief SysUptime when the last packet of the flow was received
        uint32_t last;
        /// @brief TCP/UDP source port number or equivalent
        uint16_t src_port;
        /// @brief TCP/UDP destination port number or equivalent
        uint16_t dst_port;
        /// @brief Unused (zero) bytes
        uint8_t pad1;
        /// @brief Cumulative OR of TCP flags
        uint8_t tcp_flags;
        /// @brief IP protocol type
        uint8_t prot;
        /// @brief IP type of service;
        uint8_t tos;
        /// @brief Autonomous system number of the source (origin or peer)
        uint16_t src_as;
        /// @brief Autonomous system number of the destination (origin or peer)
        uint16_t dst_as;
        /// @brief Source address prefix mask bits
        uint8_t src_mask;
        /// @brief Destination address prefix mask bits
        uint8_t dst_mask;
        /// @brief Unused (zero) bytes
        uint16_t pad2;
};

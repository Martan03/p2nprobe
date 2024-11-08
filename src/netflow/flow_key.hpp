/// Martin Slezák (xsleza26)
///
/// Flow key details were found in the link below
/// https://en.wikipedia.org/wiki/NetFlow#Network_flows

#pragma once

#include <stdint.h>
#include <string_view>

struct __attribute__ ((packed)) FlowKey {
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
};

template<> struct std::hash<FlowKey>
{
    std::size_t operator()(const FlowKey& key) const noexcept
    {
        std::string_view str_view(
            reinterpret_cast<const char*>(&key),
            sizeof(key)
        );
        return std::hash<std::string_view>()(str_view);
    }
};

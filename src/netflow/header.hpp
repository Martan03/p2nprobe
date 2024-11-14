/// Martin Slezák (xsleza26)
///
/// Header details were found on the link below
/// https://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html#wp1006108

#pragma once

#include <cstdint>
#include <chrono>
#include <netinet/in.h>

struct __attribute__ ((packed)) NetflowV5Header {
    /// @brief Version of NetFlow (probably won't need this)
    uint16_t version = htons(5);
    /// @brief Number of flows exported in packet (1-30)
    uint16_t count = 0;
    /// @brief Milliseconds since export device booted
    uint32_t sys_uptime = 0;
    /// @brief Count of seconds since 0000 UTC 1970
    uint32_t unix_secs;
    /// @brief Residual nanoseconds since 0000 UTC 1970
    uint32_t unix_nsecs;
    /// @brief  Sequence counter of total flows seen
    uint32_t flow_sequence = 0;
    /// @brief Type of flow-switching engine
    uint8_t engine_type = 0;
    /// @brief Slot number of flow-switching engine
    uint8_t engine_id = 0;
    /// @brief 2-bits sampling mode, 14-bits value of sampling interval
    uint16_t sampling_interval = 0;

    NetflowV5Header() {
        using namespace std::chrono;
        auto now = system_clock::now().time_since_epoch();
        auto secs = duration_cast<seconds>(now);
        auto nsecs = duration_cast<nanoseconds>(now - secs);

        unix_secs = static_cast<uint32_t>(secs.count());
        unix_nsecs = static_cast<uint32_t>(nsecs.count());
    }

    /// @brief Converts values from little endian to big endian
    void endian_prep() {
        count = htons(count);
        sys_uptime = htonl(sys_uptime);
        unix_secs = htonl(unix_secs);
        unix_nsecs = htonl(unix_nsecs);
        flow_sequence = htonl(flow_sequence);
    }

    /// @brief Converts values from big endian to little endian
    void endian_unprep() {
        count = ntohs(count);
        sys_uptime = ntohl(sys_uptime);
        unix_secs = ntohl(unix_secs);
        unix_nsecs = ntohl(unix_nsecs);
        flow_sequence = ntohl(flow_sequence);
    }
};

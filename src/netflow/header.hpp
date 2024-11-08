/// Martin Slezák (xsleza26)
///
/// Header details were found on the link below
/// https://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html#wp1006108

#pragma once

#include <stdint.h>

struct NetflowV5Header {
    /// @brief Version of NetFlow (probably won't need this)
    uint16_t version;
    /// @brief Number of flows exported in packet (1-30)
    uint16_t count;
    /// @brief Milliseconds since export device booted
    uint32_t sys_uptime;
    /// @brief Count of seconds since 0000 UTC 1970
    uint32_t unix_secs;
    /// @brief Residual nanoseconds since 0000 UTC 1970
    uint32_t unix_nsecs;
    /// @brief  Sequence counter of total flows seen
    uint32_t flow_sequence;
    /// @brief Type of flow-switching engine
    uint8_t engine_type;
    /// @brief Slot number of flow-switching engine
    uint8_t engine_id;
    /// @brief 2-bits sampling mode, 14-bits value of sampling interval
    uint16_t sampling_interval;
};

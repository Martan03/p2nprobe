/// Martin Slezák (xsleza26)

#pragma once

#include <stdint.h>

class NetflowV5Header {
    public:
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
        /// @brief  Sequence counter of totatl flows seen
        uint32_t flow_sequence;
        /// @brief Type of flow-switching engine
        uint8_t engine_type;
        /// @brief Slot number of flow-switching engine
        uint8_t engine_id;
        /// @brief 2-bits sampling mode, 14-bits value of sampling interval
        uint16_t sampling_interval;
};

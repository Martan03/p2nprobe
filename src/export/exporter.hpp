/// Martin Slezák (xsleza26)

#pragma once

#include <vector>
#include <chrono>

#include "udp_client.hpp"
#include "../args.hpp"
#include "../netflow/flow.hpp"
#include "../netflow/netflow.hpp"
#include "../netflow/header.hpp"

class Exporter {
    public:
        std::chrono::system_clock::time_point uptime;

        Exporter(Args args);

        /// @brief Exports given flow
        /// @param flow flow to be exported
        void export_flow(Flow flow);

        /// @brief Sends all flows exporter has
        void flush();

    private:
        UdpClient client;
        std::vector<NetflowV5Flow> flows;
        NetflowV5Header header;

        /// @brief Gets elapsed milliseconds since exporter started
        /// @return milliseconds since exported started
        uint32_t get_elapsed_ms();
};

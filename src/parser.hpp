/// Martin Slezák (xsleza26)

#pragma once

#include <pcap.h>
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <sys/time.h>

#include "args.hpp"
#include "netflow/flow_key.hpp"
#include "netflow/flow.hpp"
#include "netflow/netflow.hpp"
#include "netflow/header.hpp"
#include "udp_client.hpp"

class Parser {
    public:
        Parser(Args args);

        void parse();

    private:
        Args args;

        UdpClient client;

        pcap_t* pcap;
        char err_buf[PCAP_ERRBUF_SIZE];

        NetflowV5Header header;
        std::unordered_map<FlowKey, Flow> flows;
        std::queue<FlowKey> flow_queue;

        std::chrono::system_clock::time_point uptime;

        void process_packet(pcap_pkthdr *header, const u_char *packet);

        /// @brief Update the flow
        /// @param key flow key
        /// @param time time of arrival
        void update_flow(FlowKey key, timeval time);

        /// @brief Creates new flow and saves it to flows
        /// @param key flow key
        /// @param time time of arrival
        void create_flow(FlowKey key, timeval time);

        /// @brief Checks active timeouts on flows
        void check_actives();

        /// @brief Checks the active timeout of the flow
        /// @param flow flow to check the timeout of
        /// @param now current time
        /// @return true when timeout hit, else false
        bool check_active(
            Flow flow,
            std::chrono::system_clock::time_point now
        );

        /// @brief Check the inactive timeout of the flow
        /// @param flow flow to check the timeout of
        /// @return true when timeout hit, else false
        bool check_inactive(Flow flow);

        /// @brief Converts timeval packet arrival to chrono time point
        /// @param t timeval to be converted
        /// @return converted time point
        std::chrono::system_clock::time_point packet_time(timeval t);
};

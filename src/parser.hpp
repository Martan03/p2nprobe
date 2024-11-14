/// Martin Slezák (xsleza26)

#pragma once

#include <pcap.h>
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <sys/time.h>
#include <netinet/tcp.h>

#include "args.hpp"
#include "netflow/flow_key.hpp"
#include "netflow/flow.hpp"
#include "netflow/netflow.hpp"
#include "netflow/header.hpp"
#include "export/exporter.hpp"

class Parser {
    public:
        Parser(Args args);

        void parse();

    private:
        Args args;

        Exporter exporter;

        std::unordered_map<FlowKey, Flow> flows;
        std::queue<FlowKey> flow_queue;

        pcap_t* pcap;
        char err_buf[PCAP_ERRBUF_SIZE];

        std::chrono::system_clock::time_point uptime;

        void process_packet(pcap_pkthdr *header, const u_char *packet);

        /// @brief Update the flow
        /// @param key flow key
        /// @param time time of arrival
        void update_flow(
            FlowKey key,
            std::chrono::system_clock::time_point time,
            uint32_t size,
            const tcphdr *header
        );

        /// @brief Creates new flow and saves it to flows
        /// @param key flow key
        /// @param time time of arrival
        void create_flow(
            FlowKey key,
            std::chrono::system_clock::time_point time,
            uint32_t size,
            const tcphdr *header
        );

        /// @brief Flushes all the flows from the queue
        void flush();

        /// @brief Checks active timeouts of the flows in queue
        /// @param now time of current packet
        /// @return true when current flow was exported, else false
        void check_actives(std::chrono::system_clock::time_point now);

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
        bool check_inactive(
            Flow flow,
            std::chrono::system_clock::time_point now
        );

        /// @brief Converts timeval packet arrival to chrono time point
        /// @param t timeval to be converted
        /// @return converted time point
        std::chrono::system_clock::time_point packet_time(timeval t);
};

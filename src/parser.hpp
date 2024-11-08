/// Martin Slezák (xsleza26)

#pragma once

#include <pcap.h>
#include <string>
#include <unordered_map>
#include <sys/time.h>

#include "netflow/flow_key.hpp"
#include "netflow/flow.hpp"
#include "netflow/header.hpp"

class Parser {
    public:
        Parser(std::string file);

        void parse();

    private:
        pcap_t* pcap;
        char err_buf[PCAP_ERRBUF_SIZE];

        NetflowV5Header header;
        std::unordered_map<FlowKey, NetflowV5Flow> flows;

        timeval uptime;

        void process_packet(
            struct pcap_pkthdr *header,
            const u_char *packet
        );

        uint32_t get_timestamp(timeval time);
};

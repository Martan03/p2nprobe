/// Martin Slezák (xsleza26)

#pragma once

#include <pcap.h>
#include <string>

class Parser {
    public:
        Parser(std::string file);

        void parse();

    private:
        pcap_t* pcap;
        char err_buf[PCAP_ERRBUF_SIZE];

        void process_packet(
            struct pcap_pkthdr *header,
            const u_char *packet,
            int cnt
        );
};

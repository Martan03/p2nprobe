/// Martin Slezák (xsleza26)

#pragma once

#include <pcap.h>
#include <string>

class Parser {
    public:
        Parser(std::string file);

    private:
        pcap_t* pcap;
        char err_buf[PCAP_ERRBUF_SIZE];
};

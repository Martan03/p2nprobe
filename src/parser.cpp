/// Martin Slezák (xsleza26)

#include "parser.hpp"

Parser::Parser(std::string file) {
    pcap = pcap_open_offline(file.c_str(), err_buf);
}

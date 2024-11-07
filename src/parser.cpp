/// Martin Slezák (xsleza26)

#include "parser.hpp"

#include <stdexcept>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#define ETH_ADDR_LEN 6
#define ETH_HEADER_LEN 14

struct Ethernet {
    u_char dhost[ETH_ADDR_LEN];
    u_char shost[ETH_ADDR_LEN];
    u_short type;
};

#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

Parser::Parser(std::string file) {
    pcap = pcap_open_offline(file.c_str(), err_buf);
    if (pcap == nullptr) {
        throw std::runtime_error("Could not open PCAP file");
    }
}

void Parser::parse() {
    struct pcap_pkthdr *header;
    const u_char *packet;
    int cnt = 0;

    while (int ret = pcap_next_ex(pcap, &header, &packet) >= 0) {
        if (ret == 0)
            continue;

        process_packet(header, packet, cnt++);
    }

    pcap_close(pcap);
}

void Parser::process_packet(
    struct pcap_pkthdr *header,
    const u_char *packet,
    int cnt
) {
    struct ether_header *eth = (struct ether_header*)packet;
    struct ip *ip_header = (struct ip*)(packet + ETH_HEADER_LEN);
    int ip_header_len = ip_header->ip_hl * 4;
    if (ip_header_len < 20) {
        // std::cout <<
        //     "Invalid IP header length: " << ip_header_len << " bytes"
        // << std::endl;
        return;
    }

    if (ip_header->ip_p != IPPROTO_TCP) {
        return;
    }

    std::cout << "Packet #" << cnt << std::endl;

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);
    std::cout <<
        "Source IP: " << src_ip <<
        ", Destination IP: " << dst_ip
    << std::endl;

    struct tcphdr *tcp_header =
        (struct tcphdr*)(packet + ETH_HEADER_LEN + ip_header_len);
    std::cout <<
        "Protocol: TCP, Source Port: " << ntohs(tcp_header->source) <<
        ", Destination Port: " << ntohs(tcp_header->dest)
    << std::endl;

    std::cout << "Packet length: " << header->len << " bytes" << std::endl;
}

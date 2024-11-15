/// Martin Slezák (xsleza26)

#include "parser.hpp"

#include <stdexcept>
#include <iostream>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#define ETH_ADDR_LEN 6
#define ETH_HEADER_LEN 14

Parser::Parser(Args args):
    args(args),
    exporter(args),
    flows()
{
    pcap = pcap_open_offline(args.file.value().c_str(), err_buf);
    if (pcap == nullptr) {
        throw std::runtime_error("Could not open PCAP file");
    }

    uptime = std::chrono::system_clock::now();
}

void Parser::parse() {
    pcap_pkthdr *header;
    const u_char *packet;

    while (int ret = pcap_next_ex(pcap, &header, &packet) >= 0) {
        if (ret == 0)
            continue;

        process_packet(header, packet);
    }

    flush();
    exporter.flush();
    pcap_close(pcap);
}

/// This function was inspired by code on this website:
/// https://www.tcpdump.org/pcap.html
void Parser::process_packet(pcap_pkthdr *header, const u_char *packet) {
    auto eth = reinterpret_cast<const ether_header*>(packet);
    // Skips packets that are not IP packets
    auto eth_type = ntohs(eth->ether_type);
    if (eth_type != ETHERTYPE_IP) {
        return;
    }

    Packet parsed;
    parsed.size = header->len - ETH_HEADER_LEN;
    parsed.time = packet_time(header->ts);

    process_ip(parsed, packet + ETH_HEADER_LEN);
}

void Parser::process_ip(Packet parsed, const u_char *packet) {
    auto ip_header = reinterpret_cast<const ip*>(packet);
    int ip_header_len = ip_header->ip_hl * 4;
    // Stops processing when invalid IP header or not TCP
    if (ip_header_len < 20 || ip_header->ip_p != IPPROTO_TCP) {
        return;
    }

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);
    parsed.src_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_src);
    parsed.dst_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_dst);

    parsed.tos = ip_header->ip_tos;

    process_tcp(parsed, packet + ip_header_len);
}

void Parser::process_tcp(Packet parsed, const u_char *packet) {
    auto tcp_header = reinterpret_cast<const tcphdr*>(packet);

    parsed.src_port = tcp_header->source;
    parsed.dst_port = tcp_header->dest;
    parsed.tcp_flags = tcp_header->th_flags;

    FlowKey key(parsed);
    if (flows.contains(key)) {
        update_flow(key, parsed);
    } else {
        create_flow(key, parsed);
    }

    check_actives(parsed.time);
}

void Parser::update_flow(FlowKey key, Packet parsed) {
    auto flow = flows[key];

    if (check_inactive(flow, parsed.time) || check_active(flow, parsed.time)) {
        exporter.export_flow(flow);
        Flow flow(parsed);
        flows[key] = flow;
        return;
    }

    flow.d_pkts++;
    flow.d_octets += parsed.size;
    flow.last = parsed.time;
    flow.tcp_flags |= parsed.tcp_flags;
    flows[key] = flow;
}

void Parser::create_flow(FlowKey key, Packet parsed) {
    Flow flow(parsed);
    flow.d_octets += parsed.size;
    flow.tcp_flags = parsed.tcp_flags;
    flows[key] = flow;
    flow_queue.push(key);
}

void Parser::flush() {
    while (!flow_queue.empty()) {
        auto flow_key = flow_queue.front();
        exporter.export_flow(flows[flow_key]);
        flow_queue.pop();
        flows.erase(flow_key);
    }
}

void Parser::check_actives(std::chrono::system_clock::time_point n) {
    while (!flow_queue.empty() && check_active(flows[flow_queue.front()], n)) {
        auto flow_key = flow_queue.front();
        exporter.export_flow(flows[flow_key]);
        flow_queue.pop();
        flows.erase(flow_key);
    }
}

bool Parser::check_active(
    Flow flow,
    std::chrono::system_clock::time_point now
) {
    using namespace std::chrono;
    auto secs = now - flow.first;
    return secs >= args.active;
}

bool Parser::check_inactive(
    Flow flow,
    std::chrono::system_clock::time_point now
) {
    using namespace std::chrono;
    auto secs = now - flow.first;
    return secs >= args.inactive;
}

std::chrono::system_clock::time_point Parser::packet_time(timeval t) {
    using namespace std::chrono;
    auto dur = duration_cast<system_clock::duration>(
        seconds(t.tv_sec) + microseconds(t.tv_usec)
    );
    return system_clock::time_point(dur);
}

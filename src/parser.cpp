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

void Parser::process_packet(pcap_pkthdr *header, const u_char *packet) {
    auto eth = reinterpret_cast<const ether_header*>(packet);

    auto size = header->len - ETH_HEADER_LEN;
    auto ip_header = reinterpret_cast<const ip*>(packet + ETH_HEADER_LEN);
    int ip_header_len = ip_header->ip_hl * 4;
    if (ip_header_len < 20) {
        return;
    }

    if (ip_header->ip_p != IPPROTO_TCP) {
        return;
    }

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);

    auto tcp_header = reinterpret_cast<const tcphdr*>(
        packet + ETH_HEADER_LEN + ip_header_len
    );

    FlowKey key {
        .src_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_src),
        .dst_addr = *reinterpret_cast<const uint32_t*>(&ip_header->ip_dst),
        .src_port = tcp_header->source,
        .dst_port = tcp_header->dest,
        .tos = ip_header->ip_tos,
    };

    auto now = packet_time(header->ts);
    if (flows.contains(key)) {
        update_flow(key, now, size, tcp_header);
    } else {
        create_flow(key, now, size, tcp_header);
    }

    check_actives(now);
}

void Parser::update_flow(
    FlowKey key,
    std::chrono::system_clock::time_point time,
    uint32_t size,
    const tcphdr *header
) {
    auto flow = flows[key];

    if (check_inactive(flow, time) || check_active(flow, time)) {
        exporter.export_flow(flow);
        Flow flow(key, time);
        flow.tcp_flags = header->th_flags;
        flows[key] = flow;
        return;
    }

    // TODO: edit all values
    flow.d_pkts++;
    flow.last = time;
    flow.tcp_flags |= header->th_flags;
    flows[key] = flow;
}

void Parser::create_flow(
    FlowKey key,
    std::chrono::system_clock::time_point time,
    uint32_t size,
    const tcphdr *header
) {
    // TODO: add all values
    Flow flow(key, time);
    flow.tcp_flags = header->th_flags;
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

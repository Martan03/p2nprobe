/// Martin Slezák (xsleza26)

#include "exporter.hpp"

#include <cstring>
#include <iostream>

Exporter::Exporter(Args args):
    uptime(std::chrono::system_clock::now()),
    client(args),
    flows(),
    header()
{}

void Exporter::export_flow(Flow flow) {
    NetflowV5Flow netflow(flow, uptime);
    flows.push_back(netflow);

    header.count++;
    header.flow_sequence++;

    if (flows.size() == 29) {
        flush();
    }
}

void Exporter::flush() {
    header.sys_uptime = get_elapsed_ms();

    auto flows_size = flows.size() * sizeof(NetflowV5Flow);
    std::vector<char> buffer(sizeof(NetflowV5Header) + flows_size);

    header.endian_prep();
    memcpy(buffer.data(), &header, sizeof(NetflowV5Header));
    header.endian_unprep();
    memcpy(buffer.data() + sizeof(NetflowV5Header), flows.data(), flows_size);

    client.send(buffer);

    header.count = 0;
    flows.clear();

    std::cout << "Flows sent" << std::endl;
}

uint32_t Exporter::get_elapsed_ms() {
    using namespace std::chrono;
    auto msecs = duration_cast<milliseconds>(system_clock::now() - uptime);
    return uint32_t(msecs.count());
}

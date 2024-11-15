/// Martin Slezák (xsleza26)

#include <cstring>
#include <iostream>
#include <span>

#include "args.hpp"

Args Args::parse(std::span<char*> argv) {
    Args args;
    argv = argv.subspan(1);

    for (auto arg = argv.begin(); arg != argv.end(); ++arg) {
        if (std::strcmp(*arg, "-a") == 0) {
            args.active = std::chrono::seconds(
                Args::parse_num(arg, argv.end())
            );
        } else if (std::strcmp(*arg, "-i") == 0) {
            args.inactive = std::chrono::seconds(
                Args::parse_num(arg, argv.end())
            );
        } else if (
            std::strcmp(*arg, "-h") == 0 ||
            std::strcmp(*arg, "--help") == 0
        ) {
            args.help = true;
        } else if (!args.parse_host(*arg)) {
            args.file = *arg;
        }
    }

    return args;
}

void Args::check_required() {
    if (!file.has_value()) {
        throw std::invalid_argument("Missing required PCAP file path");
    } else if (!host.has_value() || !port.has_value()) {
        throw std::invalid_argument(
            "Hostname and port required (in format hostaname:port)"
        );
    }
}

std::string Args::next_arg(
    std::span<char*>::iterator& arg,
    std::span<char*>::iterator end
) {
    std::string arg_cmd = *arg;
    if (++arg == end)
        throw std::invalid_argument("'" + arg_cmd + "' expects parameter");

    return *arg;
}

unsigned Args::parse_num(
    std::span<char*>::iterator& arg,
    std::span<char*>::iterator end
) {
    std::string cmd = *arg;
    auto val = Args::next_arg(arg, end);
    try {
        return std::stoul(val);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("'" + cmd + "' expects number argument");
    }
}

bool Args::parse_host(std::string arg) {
    std::size_t pos = arg.find(':');
    if (pos == std::string::npos) {
        return false;
    }

    auto host = arg.substr(0, pos);
    try {
        this->port = std::stoul(arg.substr(pos + 1));
        this->host = host;
    } catch (const std::invalid_argument&) {
        return false;
    }

    return true;
}

void Args::print_help() {
    std::cout <<
        "Usage:\n"
        "  ./p2nprobe <host>:<port> <pcap_file_path> [OPTION...]\n"
        "  ./p2nprobe -h\n"
        "Options:\n"
        "  -a <active_timeout>\n"
        "    Number of seconds to set active timeout of flow export to\n"
        "  -i <inactive_timeout>\n"
        "    Number of seconds to set inactive timeout of flow export to\n"
        "  -h  --help\n"
        "    Prints this help (other arguments are ignored)\n";
}

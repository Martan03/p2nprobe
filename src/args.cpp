#include <cstring>
#include <iostream>

#include "args.hpp"

Args Args::parse(int argc, char** argv) {
    Args args;
    while (*++argv) {
        char* pos;
        if (std::strcmp(*argv, "-a") == 0) {
            args.timeout = Args::parse_num(*argv, *++argv);
        } else if (std::strcmp(*argv, "-i") == 0) {
            args.inactive = Args::parse_num(*argv, *++argv);
        } else if (!args.parse_host(*argv)) {
            args.file = *argv;
        }
    }

    return args;
}

unsigned Args::parse_num(std::string arg, char* val) {
    try {
        return std::stoul(val);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("'" + arg + "' expects number argument");
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

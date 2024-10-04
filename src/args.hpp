#pragma once

#include <cstdint>
#include <string>
#include <optional>

class Args {
    public:
        std::optional<std::string> file;
        std::optional<std::string> host;
        std::optional<std::uint16_t> port;
        unsigned timeout = 60;
        unsigned inactive = 60;

        static Args parse(int argc, char** argv);

    private:
        static unsigned parse_num(std::string arg, char* val);

        bool parse_host(std::string arg);
};
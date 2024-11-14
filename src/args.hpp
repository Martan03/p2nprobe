/// Martin Slezák (xsleza26)

#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <span>
#include <chrono>

class Args {
    public:
        /// @brief PCAP file path
        std::optional<std::string> file;
        /// @brief host to send the flow to
        std::optional<std::string> host;
        /// @brief port to send the flow to
        std::optional<std::uint16_t> port;

        /// @brief active timeout
        std::chrono::seconds active { 60 };
        /// @brief inactive timeout
        std::chrono::seconds inactive { 60 };

        /// @brief whether to show help or not
        bool help = false;

        /// @brief Parses the given arguments
        /// @param argv
        /// @return parsed arguments
        static Args parse(std::span<char*> argv);

        /// @brief Prints the program help
        static void print_help();

    private:
        /// @brief Parses the host and port from given string
        /// @param arg string that should be parsed to host and port
        /// @return true on success, else false
        bool parse_host(std::string arg);

        /// @brief Gets next argument from arguments list
        /// @param arg arguments iterator
        /// @param end end of the iterator
        /// @return next argument as std::string
        static std::string next_arg(
            std::span<char*>::iterator& arg,
            std::span<char*>::iterator end
        );

        /// @brief Read next argument and parses it as unsigned number
        /// @param arg argument iterator
        /// @param end end of the iterator
        /// @return parsed unsigned number
        static unsigned parse_num(
            std::span<char*>::iterator& arg,
            std::span<char*>::iterator end
        );
};
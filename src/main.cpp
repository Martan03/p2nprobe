/// Martin Slezák (xsleza26)

#include <iostream>

#include "args.hpp"

void run(std::span<char*> argv);

int main(int argc, char** argv) {
    try {
        run({argv, static_cast<unsigned long>(argc)});
        std::cout << "Success" << std::endl;
        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

}

void run(std::span<char*> argv) {
    Args args = Args::parse(argv);
    if (args.help) {
        Args::print_help();
        return;
    }

    std::cout << args.file.value_or("Not set") << std::endl;
    std::cout << args.host.value_or("Not set") << ":" << args.port.value_or(0) << std::endl;
    std::cout << args.active << std::endl;
    std::cout << args.inactive << std::endl;
}

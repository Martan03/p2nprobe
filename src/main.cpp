/// Martin Slezák (xsleza26)

#include <iostream>

#include "args.hpp"
#include "parser.hpp"

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

    if (args.file.has_value()) {
        auto parser = new Parser(args);
        parser->parse();
    }
}

/// Martin Slezák (xsleza26)

#include <iostream>
#include <cstdint>

#include "receiver.hpp"

void run(int argc, char **argv);

int main(int argc, char** argv) {
    try {
        run(argc, argv);
        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void run(int argc, char **argv) {
    if (argc != 3) {
        throw std::invalid_argument("hostname and port required");
    }

    Receiver rec(argv[1], argv[2]);
    rec.start();
}

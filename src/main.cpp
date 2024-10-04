#include <iostream>

#include "args.hpp"

void run(int argc, char** argv) {
    Args args = Args::parse(argc, argv);
    std::cout << args.file.value_or("Not set") << std::endl;
    std::cout << args.host.value_or("Not set") << ":" << args.port.value_or(0) << std::endl;
}

int main(int argc, char** argv) {
    try {
        run(argc, argv);
        std::cout << "Success" << std::endl;
        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

}

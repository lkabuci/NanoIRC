#include <iostream>
#include "srcs/parser/Message.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <password> <command>"
                  << std::endl;
        return 1;
    }
    try {
        Message parser(std::string(argv[2]) + "\r\n");

        parser.execute(argv[1]);
        // std::cout << parser.get() << '\n';
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

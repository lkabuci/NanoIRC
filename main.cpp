#include "include/IRC.h"
#include "srcs/Socket.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return 1;
    }

    Socket s(argv[1]);
    std::cout << "socket fd: " << s.getSocketFd() << std::endl;
    std::cout << "ip: " << s.getIP() << std::endl;
    std::cout << "port: " << s.getPort() << std::endl;

    return 0;
}

#include "SignalHandler.hpp"

SignalHandler::SignalHandler() {
    struct sigaction sigIntHandler;
    std::memset(&sigIntHandler, 0, sizeof(sigIntHandler));
    sigIntHandler.sa_handler = &SignalHandler::handleSIGINT;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

void SignalHandler::handleSIGINT(int signum) {
    (void)signum;
    std::cout << "Received SIGINT\n";
    serverIsRunning = false;
}

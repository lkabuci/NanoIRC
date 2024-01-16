//
// Created by Redouane Elkaboussi on 1/15/24.
//

#include "AddressResolver.hpp"

AddressResolver::AddressResolver(const char* port)
    : _addresses(NULL), _port(port) {
    fillAddressInfo();
}

const struct addrinfo* AddressResolver::getAddresses() const {
    return _addresses;
}

void AddressResolver::fillAddressInfo() {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPV4 and IPV6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    const int rv = getaddrinfo(NULL, _port, &hints, &_addresses);
    if (rv != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << ".\n";
        std::exit(EXIT_FAILURE);
    }
}

AddressResolver::~AddressResolver() {
    freeaddrinfo(_addresses);
    _addresses = NULL;
}

//
// Created by Redouane Elkaboussi on 1/15/24.
//

#pragma once

#include <netdb.h>
#include <vector>

class AddressResolver {
  public:
    explicit AddressResolver(const char* port);
    ~AddressResolver();
    const struct addrinfo* getAddresses() const;

  private:
    struct addrinfo* _addresses;
    const char*      _port;

    void fillAddressInfo();

    AddressResolver(const AddressResolver&);
    AddressResolver& operator=(AddressResolver&);
};

//
// Created by Redouane Elkaboussi on 1/16/24.
//

#ifndef IRC_CLIENTHANDLER_HPP
#define IRC_CLIENTHANDLER_HPP

#include "Client.hpp"

class ClientHandler {
  public:
    static void handleNewConnection(const pollfd& serverFd);
    static void handleClientInput(Client*& pClient);
    static void handleReceiveError(Client*& pClient);
    static void handleTooLongMessage(Client*& pClient);
    static bool hasEndOfMessage(const std::string& message);
};

#endif // IRC_CLIENTHANDLER_HPP

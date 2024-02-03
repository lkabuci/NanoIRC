#include "Reply.hpp"
#include "../Utils/Utils.hpp"

void Reply::errUnknownCommand(Client* client, const std::string& cmd) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 421 " +
                        client->getUserInfo().getNickname() + " " + cmd +
                        " :Unknown command\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNotEnoughParams(Client* client, const std::string& cmd) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 461 " +
                        client->getUserInfo().getNickname() + " " + cmd +
                        " :Not enough parameters\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNoSuchChannel(Client* client, const std::string& name) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 403 " +
                        client->getUserInfo().getNickname() + " " + name +
                        " :No such channel\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errInviteOnlyChan(Client* client, const std::string& name) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 473 " +
                        client->getUserInfo().getNickname() + " " + name +
                        " :Cannot join channel, you must be invited (+i)\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errChannelIsFull(Client* client, const std::string& name) {
    std::string msg = std::string(":") + Reactor::getServerName() + " 471 " +
                      client->getUserInfo().getNickname() + " " + name +
                      " :Cannot join channel, Channel is full (+l)\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
}

void Reply::errBadChannelKey(Client* client, const std::string& name) {
    std::string reply =
        std::string(":") + Reactor::getServerName() + " 475 " +
        client->getUserInfo().getNickname() + " " + name +
        " :Cannot join channel, you need the correct key (+k)\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::channelReply(Client* client, const std::string& name) {
    std::string msg1;
    std::string msg2;
    std::string msg3;

    msg1 = ":" + client->getUserInfo().getNickname() + "!" +
           client->getUserInfo().getUsername() + "@" + client->getIp() +
           " JOIN " + name + CR_LF;

    msg2 = std::string(":") + Reactor::getServerName() + " 353 " +
           client->getUserInfo().getNickname() + " = " + name + " :@" +
           client->getUserInfo().getNickname() + CR_LF;

    msg3 = std::string(":") + Reactor::getServerName() + " 366 " +
           client->getUserInfo().getNickname() + " " + name +
           " :End of /NAMES list.\r\n";

    send(client->getSockfd(), msg1.c_str(), msg1.length(), 0);
    send(client->getSockfd(), msg2.c_str(), msg2.length(), 0);
    send(client->getSockfd(), msg3.c_str(), msg3.length(), 0);
}

void Reply::welcome(Client* client) {
    std::string server_name = std::string(":") + Reactor::getServerName();
    std::string rpl_welcome =
        server_name + " 001 " + client->getUserInfo().getNickname() +
        " :Welcome to the Internet Relay Network " +
        client->getUserInfo().getNickname() + "!" +
        client->getUserInfo().getUsername() + "@" + client->getIp() + CR_LF;
    std::string rpl_yourhost = server_name + " 002 " +
                               client->getUserInfo().getNickname() +
                               " :Your host is " + Reactor::getServerName() +
                               ", running version i1\r\n";
    std::string rpl_created =
        server_name + " 003 " + client->getUserInfo().getNickname() +
        " :This server was created Tue 16 Jan 2024 at 14:47\r\n";
    std::string rpl_myinfo = server_name + " 004 " +
                             client->getUserInfo().getNickname() + " " +
                             server_name + " i1 itkol\r\n";

    send(client->getSockfd(), rpl_welcome.c_str(), rpl_welcome.length(), 0);
    send(client->getSockfd(), rpl_yourhost.c_str(), rpl_yourhost.length(), 0);
    send(client->getSockfd(), rpl_created.c_str(), rpl_created.length(), 0);
    send(client->getSockfd(), rpl_myinfo.c_str(), rpl_myinfo.length(), 0);
}

void Reply::errPasswdMismatch(Client* client) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 464 " +
                        client->getUserInfo().getNickname() +
                        " :Password incorrect\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errAlreadyRegistred(Client* client) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 462 " +
                        client->getUserInfo().getNickname() +
                        " :You may not reregister\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNoSuchNick(Client* client, const std::string& name) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 401 " +
                        client->getUserInfo().getNickname() + " " + name +
                        " :No such nick\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNotRegistered(Client* client) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 451 ";

    if (client->getUserInfo().getNickname().empty()) {
        reply.append("*");
    } else {
        reply.append(client->getUserInfo().getNickname() + " " +
                     client->getUserInfo().getUsername());
    }
    reply.append(" :You have not regisetred\r\n");

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNoTextToSend(Client* client) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 412 " +
                        client->getUserInfo().getNickname() +
                        " :No text to send\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errNoRecipient(Client* client, const std::string& cmd) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 411 " +
                        client->getUserInfo().getNickname() +
                        " :No recipent given (" + cmd + ")\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errErroneousNickname(Client* client, const std::string& name) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 432 ";

    if (client->getUserInfo().isSet(UserInfo::NICK_SET)) {
        reply.append(client->getUserInfo().getNickname());
    } else {
        reply.append("*");
    }
    reply.append(" " + name + " :Erroneouse Nickname\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void Reply::errCannotSendToChan(Client* client, const std::string& channel,
                                const std::string& nickname) {
    std::string reply = std::string(":") + Reactor::getServerName() + " 404 " +
                        nickname + " " + channel +
                        " :Cannot send to channel\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

//
// Created by Redouane Elkaboussi on 1/15/24.
//

#ifndef IRC_IRC_H
#define IRC_IRC_H

#define CR_LF "\r\n"
#define LF_ '\n'

#include <csignal>
extern volatile sig_atomic_t serverIsRunning;

#include "../srcs/Utils/Utils.hpp"
#include "../srcs/parser/TokenType.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#endif // IRC_IRC_H

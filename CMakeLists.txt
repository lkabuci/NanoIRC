cmake_minimum_required(VERSION 3.26)
project(IRC)

set(CMAKE_CXX_STANDARD 98)
file (GLOB_RECURSE IRC_SRCS
        main.cpp
        srcs/*.cpp
        srcs/*/*.cpp
)

add_executable(ircserv ${IRC_SRCS})
target_compile_options(ircserv PRIVATE -Wall -Wextra -g)

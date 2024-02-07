GREEN = \033[38;2;0;153;0m
NC = \033[0m # No Color

CXX			= c++
CXXFLAGS	= -std=c++98 -Wall -Wextra -Werror

RM			= rm -rf


SRC_DIR		= srcs
BUILD_DIR	= build

NAME		= $(BUILD_DIR)/ircserv

SRCS		= ./srcs/channels/Channel.cpp \
			  ./srcs/channels/TChannels.cpp \
			  ./srcs/client/Client.cpp \
			  ./srcs/client/ClientHandler.cpp \
			  ./srcs/client/ClientList.cpp \
			  ./srcs/client/Clients.cpp \
			  ./srcs/client/UserInfo.cpp \
			  ./srcs/commands/INVITE.cpp \
			  ./srcs/commands/JOIN.cpp \
			  ./srcs/commands/KICK.cpp \
			  ./srcs/commands/MODE.cpp \
			  ./srcs/commands/NICK.cpp \
			  ./srcs/commands/NOTICE.cpp \
			  ./srcs/commands/PASS.cpp \
			  ./srcs/commands/PRIVMSG.cpp \
			  ./srcs/commands/QUIT.cpp \
			  ./srcs/commands/SendMsg.cpp \
			  ./srcs/commands/TOPIC.cpp \
			  ./srcs/commands/USER.cpp \
			  ./srcs/main.cpp \
			  ./srcs/parser/Lexer.cpp \
			  ./srcs/parser/Message.cpp \
			  ./srcs/parser/Parser.cpp \
			  ./srcs/parser/Token.cpp \
			  ./srcs/reply/Reply.cpp \
			  ./srcs/server/Demultiplexer.cpp \
			  ./srcs/server/Dispatcher.cpp \
			  ./srcs/server/Reactor.cpp \
			  ./srcs/server/Server.cpp \
			  ./srcs/server/ServerHelper.cpp \
			  ./srcs/socket/AddressResolver.cpp \
			  ./srcs/socket/Socket.cpp \
			  ./srcs/Utils/SignalHandler.cpp \
			  ./srcs/bot/Timer.cpp \
			  ./srcs/Utils/Utils.cpp

OBJECTS		= $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:%.cpp=%.o)))

INCLUDE		= include/IRC.h \
			  ./srcs/bot/Timer.hpp \
			  ./srcs/channels/Channel.hpp \
			  ./srcs/channels/TChannels.hpp \
			  ./srcs/client/Client.hpp \
			  ./srcs/client/ClientHandler.hpp \
			  ./srcs/client/ClientList.hpp \
			  ./srcs/client/Clients.hpp \
			  ./srcs/client/UserInfo.hpp \
			  ./srcs/commands/Command.hpp \
			  ./srcs/commands/INVITE.hpp \
			  ./srcs/commands/JOIN.hpp \
			  ./srcs/commands/KICK.hpp \
			  ./srcs/commands/MODE.hpp \
			  ./srcs/commands/NICK.hpp \
			  ./srcs/commands/NOTICE.hpp \
			  ./srcs/commands/PASS.hpp \
			  ./srcs/commands/PRIVMSG.hpp \
			  ./srcs/commands/QUIT.hpp \
			  ./srcs/commands/SendMsg.hpp \
			  ./srcs/commands/TOPIC.hpp \
			  ./srcs/commands/USER.hpp \
			  ./srcs/parser/Lexer.hpp \
			  ./srcs/parser/Message.hpp \
			  ./srcs/parser/Parser.hpp \
			  ./srcs/parser/Token.hpp \
			  ./srcs/parser/TokenType.hpp \
			  ./srcs/reply/Reply.hpp \
			  ./srcs/server/Demultiplexer.hpp \
			  ./srcs/server/Dispatcher.hpp \
			  ./srcs/server/Reactor.hpp \
			  ./srcs/server/Server.hpp \
			  ./srcs/server/ServerHelper.hpp \
			  ./srcs/socket/AddressResolver.hpp \
			  ./srcs/socket/Socket.hpp \
			  ./srcs/Utils/SignalHandler.hpp \
			  ./srcs/Utils/Utils.hpp

all:	$(NAME)

$(NAME): $(OBJECTS)
	@echo $(CXX) $^ -o $@
	@$(CXX) $^ -o $@

$(OBJECTS): | $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE)
	@echo "$(GREEN)=> Compiling$(NC) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/*/%.cpp $(INCLUDE)
	@echo "$(GREEN)=> Compiling$(NC) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	@$(RM) $(BUILD_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

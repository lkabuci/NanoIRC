GREEN = \033[38;2;0;153;0m
NC = \033[0m # No Color

CXX			= c++
CXXFLAGS	= -std=c++98 -Wall -Wextra -Werror -g

RM			= rm -rf


SRC_DIR		= srcs
BUILD_DIR	= build

NAME		= $(BUILD_DIR)/ircserv

SRCS		= $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS		= $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:%.cpp=%.o)))

INCLUDE		= include/IRC.h
INCLUDE		+= $(shell find ${SRC_DIR} -type f -name "*.hpp")

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

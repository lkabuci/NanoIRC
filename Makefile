# Colors
GREEN	= \x1b[32m
NC		= \x1b[0m # No Color

CXX			= c++
CXXFLAGS	= -std=c++98 -Wall -Wextra -Werror
RM			= rm -rf
SRC_DIR		= srcs
BUILD_DIR	= build

NAME		= $(BUILD_DIR)/ircserv

# Create source file list maintaining directory structure
SRCS		= $(shell find $(SRC_DIR) -name '*.cpp')

# Create object files list with build directory path
OBJECTS		= $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Create directory list for object files
OBJ_DIRS	= $(sort $(dir $(OBJECTS)))

INCLUDE		= include/IRC.h \
			  $(shell find $(SRC_DIR) -name '*.hpp')

all:	$(NAME)

$(NAME): $(OBJECTS)
	@printf "$(GREEN)=> Linking$(NC) $@\n"
	@$(CXX) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE)
	@printf "$(GREEN)=> Compiling$(NC) $<\n"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(OBJ_DIRS)

clean:
	@$(RM) $(BUILD_DIR)
	@printf "$(GREEN)=> Cleaned build files$(NC)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(GREEN)=> Cleaned executable$(NC)\n"

re: fclean all

.PHONY: all clean fclean re

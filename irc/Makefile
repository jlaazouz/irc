NAME = ircserv

# Colors
RESET = \033[0m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RED = \033[31m

# Get all .cpp files recursively
SRC = $(shell find . -type f -name "*.cpp")

# Get all .hpp files recursively (not strictly necessary for compilation)
INC = $(shell find . -type f -name "*.hpp")

# Define object directory and object files
OBJDIR = .Objs
OBJ = $(patsubst ./%.cpp,$(OBJDIR)/%.o,$(SRC))

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Create OBJDIR if not exists
$(OBJDIR)/%.o: %.cpp $(INC)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[Compiling]$(RESET) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Build target
$(NAME): $(OBJ)
	@echo "$(GREEN)[Linking]$(RESET) $(NAME)"
	@$(CXX) $(OBJ) -o $(NAME)
	@echo "$(BLUE)\n[Build Complete]$(RESET)"

# Clean and recompile
clean:
	@echo "$(RED)[Cleaning Objects]$(RESET)"
	rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)[Cleaning Executable]$(RESET)"
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re

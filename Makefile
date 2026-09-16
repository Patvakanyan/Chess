NAME := chess
OBJ_DIR := obj

CXX := c++
CPPFLAGS := -Iinclude -Iinclude/chess
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g3

SRC := $(wildcard src/*.cpp src/pieces/*.cpp src/exceptions/*.cpp)
OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR) $(NAME)

fclean: clean

re: fclean all

.PHONY: all run clean fclean re

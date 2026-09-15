NAME := chess
OBJ_DIR := obj

CXX := c++
CPPFLAGS := -Iinclude
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g3

MAIN_SRC := main.cpp
SRC := $(wildcard src/*.cpp)
OBJ := $(OBJ_DIR)/main.o $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR) $(NAME)

fclean: clean

re: fclean all

.PHONY: all run clean fclean re

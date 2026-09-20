NAME := chess
TEST_NAME := chess_tests
OBJ_DIR := obj

CXX := c++
CPPFLAGS := -Iinclude -Iinclude/chess
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g3

SRC := $(wildcard src/*.cpp src/pieces/*.cpp src/exceptions/*.cpp)
OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TEST_SRC := tests/chessUtils_test.cpp
TEST_OBJ := $(OBJ_DIR)/tests/chessUtils_test.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

$(TEST_NAME): $(filter-out $(OBJ_DIR)/main.o,$(OBJ)) $(TEST_OBJ)
	$(CXX) $^ -o $@

$(TEST_OBJ): $(TEST_SRC)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

test: $(TEST_NAME)
	./$(TEST_NAME)

clean:
	rm -rf $(OBJ_DIR) $(NAME) $(TEST_NAME)

fclean: clean

re: fclean all

.PHONY: all run test clean fclean re

NAME := Chess
TEST_NAME := Chess_tests
OBJ_DIR := obj

CXX := c++
VALGRIND := valgrind
CPPFLAGS := -Iinclude -Iinclude/Chess
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g3
CXXFLAGS += -MMD -MP

SRC := $(wildcard src/*.cpp src/pieces/*.cpp src/exceptions/*.cpp)
OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TEST_SRC := tests/ChessUtils_test.cpp
TEST_OBJ := $(OBJ_DIR)/tests/ChessUtils_test.o
DEP := $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

all: $(NAME)

.PHONY: all run makeval val test clean fclean re

-include $(DEP)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

makeval: $(NAME)
	$(VALGRIND) --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

val: makeval

$(TEST_NAME): $(filter-out $(OBJ_DIR)/Main.o,$(OBJ)) $(TEST_OBJ)
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

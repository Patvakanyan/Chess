NAME = chess
OBJ_DIR = obj

CXX = c++
CPPFLAGS = -Iinclude
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g3

SRC = main.cpp $(wildcard src/*.cpp)
OBJ = $(OBJ_DIR)/main.o $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(wildcard src/*.cpp))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR) $(NAME)

fclean: clean

re: fclean all

.PHONY: all run clean fclean re

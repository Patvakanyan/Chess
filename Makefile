CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude

BUILD_DIR := build
APP := $(BUILD_DIR)/chess
TEST_APP := $(BUILD_DIR)/test_chess

SRC := $(wildcard src/*.cpp)
TEST_SRC := $(wildcard tests/*.cpp)

.PHONY: all run test clean

ifneq ($(strip $(SRC)),)

all: $(APP)

$(BUILD_DIR):
	mkdir -p $@

$(APP): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

run: $(APP)
	./$(APP)

ifneq ($(strip $(TEST_SRC)),)
test: $(TEST_APP)

$(TEST_APP): $(filter-out src/main.cpp,$(SRC)) $(TEST_SRC) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
	./$(TEST_APP)
else
test:
	@printf '%s\n' 'No tests have been added yet.'
endif

else

all run:
	@printf '%s\n' 'No source code has been added yet.'

test:
	@printf '%s\n' 'No tests have been added yet.'

endif

clean:
	rm -rf $(BUILD_DIR)

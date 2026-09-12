CXX ?= c++
CPPFLAGS ?= -Iinclude
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic

BUILD_DIR := build
APP := $(BUILD_DIR)/chess
TEST_APP := $(BUILD_DIR)/test_chess
SOURCES := main.cpp $(wildcard src/*.cpp)
TESTS := $(wildcard tests/*.cpp)

.PHONY: all run test clean

all: $(APP)

$(APP): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

run: $(APP)
	./$(APP)

ifneq ($(strip $(TESTS)),)
test: $(TEST_APP)

$(TEST_APP): $(filter-out main.cpp,$(SOURCES)) $(TESTS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
	./$(TEST_APP)
else
test:
	@printf '%s\n' 'No tests have been added yet.'
endif

clean:
	rm -rf $(BUILD_DIR)

# Compiler
CXX = g++

# Compiler flags Flags
CXXFLAGS_BASE    = -std=c++20 -Wall -Wextra -Iinclude
CXXFLAGS_RELEASE = -O3 -DNDEBUG -flto -mtune=native -march=native
CXXFLAGS_DEBUG   = -O0 -g

# Mode (default = release)
MODE ?= release

# Directories
SRC_DIR   = src
BUILD_DIR = build/$(MODE)
BIN_DIR   = bin/$(MODE)

# Select flags
ifeq ($(MODE),debug)
    CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUG)
else
    CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_RELEASE)
endif

# Output executable path
TARGET = $(BIN_DIR)/benchmark

# Dynamically find source code files
MODULE_SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Create object files with rule: src/**/*.cpp -> build/**/*.o
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MODULE_SRCS))

# Default target
all: $(TARGET)

# Debug shortcut
debug:
	$(MAKE) MODE=debug

# Link command
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf build bin

.PHONY: all debug clean


# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2

# Project directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Output executable
TARGET = $(BIN_DIR)/benchmark

# Dynamically find source code files
MODULE_SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Create object files with rule: src/**/*.cpp -> build/**/*.o
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MODULE_SRCS))

# Default target
all: $(TARGET)

# Link command
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files command
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean


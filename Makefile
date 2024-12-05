# Compiler and flags
CXX = g++
CXXFLAGS = -Iheaders -Wall -std=c++11 -g

# Directories
SRC_DIR = src
HEADER_DIR = headers

# Output executable
TARGET = main

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Default target
all: $(TARGET)

# Link all object files into the final executable with ncurses
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) -lncurses

# Compile each .cpp file into an object file
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Phony targets
.PHONY: all clean

#all object files in the directory and the subdirectories
**/*.o
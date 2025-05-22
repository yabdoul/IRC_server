# Makefile to find all .cpp files and include a folder for includes

SRC_FILES := $(shell find . -name "*.cpp")
INCLUDE_DIR := -I./include
OBJ_FILES := $(SRC_FILES:.cpp=.o)
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 $(INCLUDE_DIR)

# Target to build the executable
TARGET := app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
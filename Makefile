# Makefile to find all .cpp files and include a folder for includes

SRC_FILES := $(shell find ./src -name "*.cpp")
INCLUDE_DIR := -I./include
OBJ_FILES := $(SRC_FILES:.cpp=.o)
CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g $(INCLUDE_DIR)
DEBUG_FLAGS := -Wall -Wextra -Werror -std=c++98 -g -O0 -DDEBUG $(INCLUDE_DIR)

# Target to build the executable
TARGET := ircserv

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(TARGET)

re: fclean all

# Debug target (no optimization, no address sanitizer for better debugging)
debug: CXXFLAGS := $(DEBUG_FLAGS)
debug: fclean $(TARGET)

.PHONY: all clean fclean re debug
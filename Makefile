CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = lexer
SOURCES = src/lexer.cpp
BUILD_DIR = build

$(BUILD_DIR)/$(TARGET): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SOURCES)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
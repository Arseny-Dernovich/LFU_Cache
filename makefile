CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = tests
SRC = tests.cpp

all: $(TARGET)

$(TARGET): $(SRC) LFU.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

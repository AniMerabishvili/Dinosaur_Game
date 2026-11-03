CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
LIBS = -lncurses
TARGET = dinosaur

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

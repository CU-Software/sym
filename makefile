CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET   = main
SOURCES  = src/main.cpp src/sym.cpp
OBJECTS  = $(SOURCES:.cpp=.o)
HEADERS  = lexer.h ast.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild

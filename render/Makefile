CXXFLAGS=-O3 -std=c++0x -Wall -Wextra 

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))

LINKS=

CXX=g++

all: $(TARGET)
	@echo renderer built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o renderer $(OBJECTS) $(LINKS)

clean:
	rm -rf $(OBJECTS) $(TARGET)

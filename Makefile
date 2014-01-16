#CXXFLAGS=-O3 -std=c++0x -Wall -Wextra -fopenmp -DDEBUG -g -pg
CXXFLAGS=-std=c++0x -Wall -Wextra

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))
LINKS=-lgmp -lgmpxx  `pkg-config glfw3 --static --cflags --libs` -lGLEW

CXX=g++

all: $(TARGET)
	@echo hardcash built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o hardcash $(OBJECTS) $(LINKS)

clean:
	rm -rf $(OBJECTS) $(TARGET)

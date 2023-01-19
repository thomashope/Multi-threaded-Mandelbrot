CXX := clang++
CXXFLAGS := $(shell sdl2-config --cflags) -std=c++11
LDFLAGS := $(shell sdl2-config --libs)
SRCS := $(wildcard *.cpp)
EXE := mandel

$(EXE):
	$(CXX) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -o mandel
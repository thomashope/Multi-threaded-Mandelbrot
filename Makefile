CXX := clang++
CXXFLAGS := $(shell sdl2-config --cflags) -std=c++11 -O3
LDFLAGS := $(shell sdl2-config --libs)
SRCS := $(wildcard ./src/*.cpp)
EXE := mandel

$(EXE): $(SRCS)
	$(CXX) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -o $(EXE)

clean:
	rm $(EXE)
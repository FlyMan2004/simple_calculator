CXX := clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -Ofast
Binary_Dir := bin

objects = calculator_core.o

all: $(objects)
	mkdir -p $(Binary_Dir)
	$(CXX) $(CXXFLAGS) -o $(Binary_Dir)/calculator $(Binary_Dir)/$^

calculator_core.o: calculator_core.cxx
	mkdir -p $(Binary_Dir)
	$(CXX) $(CXXFLAGS) -c -o $(Binary_Dir)/$@ $^
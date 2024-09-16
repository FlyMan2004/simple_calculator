CXX := clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -Ofast
Binary_Dir := bin
Test_Dir := test

objects = calculator_core.o

all: $(objects)
	mkdir -p $(Binary_Dir)
	$(CXX) $(CXXFLAGS) -o $(Binary_Dir)/calculator $(Binary_Dir)/$^

test: $(Test_Dir)/test.cxx
	mkdir -p $(Test_Dir)/tmp
	$(CXX) $(CXXFLAGS) -o $(Test_Dir)/tmp/$@ $^

calculator_core.o: calculator_core.cxx
	mkdir -p $(Binary_Dir)
	$(CXX) $(CXXFLAGS) -c -o $(Binary_Dir)/$@ $^
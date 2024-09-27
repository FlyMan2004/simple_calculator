CXX := c++
CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -O3 -ffast-math -flto -fno-exceptions
# DEBUGFLAGS := -g -fsanitize=address,undefined
DEBUGFLAGS := 
LDFLAGS := -fuse-ld=mold -Wl,--icf=all
Binary_Dir := bin
Test_Dir := test

objects = calculator_core.o

all: $(objects)
	mkdir -p $(Binary_Dir)
	$(CXX) -o $(Binary_Dir)/calculator \
		$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
		$(Binary_Dir)/$^

test: $(Test_Dir)/test.cxx
	mkdir -p $(Test_Dir)/tmp
		$(CXX) -o $(Test_Dir)/tmp/$@ \
		$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
		$^

calculator_core.o: calculator_core.cxx
	mkdir -p $(Binary_Dir)
		$(CXX) -o $(Binary_Dir)/$@ \
		$(CXXFLAGS) $(DEBUGFLAGS) -c \
		$^

compile_command:
	bear -- make all
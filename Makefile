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
	$(CXX) \
	$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
	-o $(Binary_Dir)/calculator \
	$(Binary_Dir)/$^

test: $(Test_Dir)/test.cxx
	mkdir -p $(Test_Dir)/tmp
	$(CXX) \
	$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
	-o $(Test_Dir)/tmp/$@ \
	$^

calculator_core.o: calculator_core.cxx
	mkdir -p $(Binary_Dir)
	$(CXX) \
	$(CXXFLAGS) $(DEBUGFLAGS) -c \
	-o $(Binary_Dir)/$@ \
	$^
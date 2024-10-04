CXX := c++
CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -O3 -ffast-math -flto
# DEBUGFLAGS := -Og -g -fsanitize=address,undefined
DEBUGFLAGS := 
LDFLAGS := -fuse-ld=mold -Wl,--icf=all -lstdc++exp

Binary := bin
Test := test
Src := src
Include := include
MACRO := SIMPLE_CALC

objects := 

all: $(objects)
	mkdir -p $(Binary)

test: 
	mkdir -p $(Test)

init:
	mkdir -p $(Binary) $(Test) $(Src) $(Include)

compile_command:
	bear -- $(MAKE) all
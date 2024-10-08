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

objects := calculator_core.o expr_eval.o lexer.o token.o

all: $(objects)
	mkdir -p $(Binary)
	$(CXX) -o $(Binary)/calculator \
		$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
		$(addprefix $(Binary)/, $^)

test: $(Test)/test.cxx
	mkdir -p $(Test)/tmp
	$(CXX) -o $(Test)/tmp/$@ \
		$(CXXFLAGS) $(DEBUGFLAGS) $(LDFLAGS) \
		$^

$(filter %.o,$(objects)): %.o: $(Src)/%.cxx
	mkdir -p $(Binary)
	$(CXX) -o $(Binary)/$@ \
		$(CXXFLAGS) $(DEBUGFLAGS) -c \
		$(addprefix -I,$(Include)) \
		$(addprefix -D,$(MACRO))\
		$^

compile_command:
	bear -- $(MAKE) all
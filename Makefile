CXX := clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -Ofast

calculator_core: calculator_core.cxx
	$(CXX) $(CXXFLAGS) -o calculator_core calculator_core.cxx
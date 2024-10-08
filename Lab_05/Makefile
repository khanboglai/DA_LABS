CXX = g++
CXXFLAGS =  -g -std=c++20 -pedantic -Wall -Wextra -Werror

all: lab5

lab5: main.cpp sufftree.hpp suffixtree.cpp
	$(CXX) $(CXXFLAGS) suffixtree.cpp main.cpp -o lab5

benchmark: benchmark.cpp sufftree.hpp suffixtree.cpp
	$(CXX) $(CXXFLAGS) suffixtree.cpp benchmark.cpp -o benchmark
clean:
	rm -f lab5 *.o
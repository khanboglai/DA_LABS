CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra -Werror -g

all: lab2

lab2: main.cpp btree.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o lab2

benchmark: benchmark.cpp btree.hpp
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark
clean:
	rm -f lab2 *.o benchmark
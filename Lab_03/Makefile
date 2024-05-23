CXX = g++
CXXFLAGS = -pg -std=c++20 -pedantic -Wall -Wextra -Werror

all: lab2

lab2: main.cpp btree.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o lab2

benchmark: benchmark.cpp btree.hpp
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark
clean:
	rm -f lab2 *.o benchmark
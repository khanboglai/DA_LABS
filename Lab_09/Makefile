CXX = g++
CXXFLAGS = -g -std=c++17 -pedantic -Wall -Wextra

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

benchmark: benchmark.cpp
	$(CXX)  benchmark.cpp -o bench

clean:
	rm -r *.o
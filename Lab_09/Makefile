CXX = g++
CXXFLAGS = -g -std=c++17 -pedantic -Wall -Wextra

all: main ans

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

ans: ans.cpp
	$(CXX) $(CXXFLAGS) ans.cpp -o ans

benchmark: benchmark.cpp
	$(CXX)  benchmark.cpp -o bench

clean:
	rm -r *.o
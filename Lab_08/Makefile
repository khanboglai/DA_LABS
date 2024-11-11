CXX = g++
CXXFLAGS = -g -std=c++17 -pedantic -Wall -Wextra

all: main ans

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

ans: answer.cpp
	$(CXX) $(CXXFLAGS) answer.cpp -o ans

clean:
	rm -r *.o
NAME=lab2
FILES=main.o
SOURCES=main.cpp
CC=g++
CPPFRAGS=-g -std=c++17 -pedantic -Wall
DIR=build
OBJ=$(addprefix $(DIR)/,$(FILES))


all: $(NAME)

$(NAME): dir $(FILES)
	$(CC) $(CPPFRAGS) $(OBJ) -o $(NAME)
.cpp.o:
	$(CC) $(CPPFRAGS) -c $< -o $(DIR)/$@
	

dir:
	mkdir -p $(DIR)
clean:
	rm -rf $(FILES) $(NAME)
	cd $(DIR) && rm -rf $(FILES)
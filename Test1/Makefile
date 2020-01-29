CFLAGS = -O
CC = g++
SRC = *.cpp
OBJ = $(SRC:.cpp = .o)

Game:
	g++ -c *cpp
	g++ *.o -o game -lsfml-graphics -lsfml-window -lsfml-system
	./game

clean:
	find . -name "*.o" -type f -not -name "JSONparser.o" -delete

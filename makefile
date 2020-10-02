all: snake

snake: snake.cpp
	g++ -lncurses snake.cpp -o snake

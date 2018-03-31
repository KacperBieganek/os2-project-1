all:
	g++ -std=c++11 -pthread  *.cpp -o snake.out -lncurses

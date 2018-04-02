#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <ncurses.h>
#include <mutex>

namespace snake
{

	struct Point{
		int x,y;
		Point(int x,int y) : x(x), y(y) {};
	};

	class Snake
	{
		public:
			Snake(WINDOW* win, int windowWidth, int windowHeight, std::mutex& mx,volatile bool& running) :
			       	win(win),
			       	windowWidth(windowWidth),
			       	windowHeight(windowHeight),
				mutex(mx),
				running(running)

		{
			init();
		}
			void run();

		private:
			std::vector<Point> snakePosition;
			WINDOW* win;
			std::mutex& mutex;
			int windowWidth;
			int windowHeight;
			volatile bool& running;
			void init();
			void draw();
			void move();

		
	};
}

#endif

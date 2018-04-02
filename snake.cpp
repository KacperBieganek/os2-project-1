#include "snake.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

namespace snake
{

	void Snake::run()
	{
		while(running)
		{
			move();
			{
				std::lock_guard<std::mutex> lock(mutex);
				draw();
			}
			usleep(75000);
		}
	}

	void Snake::init()
	{
		for(int i=0;i<7;i++)
		{
			snakePosition.push_back(Point{windowWidth/2+i,windowHeight/2});
		}
	}

	void Snake::draw()
	{
		wclear(win);
		box(win,'*','*');
		for(auto point : snakePosition)
		{
			mvwprintw(win,point.y,point.x,"o");
		}
		wrefresh(win);
	}

	void Snake::move()
	{
		Point tmp = snakePosition.back();
		std::rotate(snakePosition.begin(),snakePosition.begin()+1,snakePosition.end());
		int moveNS = std::rand() %3 -1;
		int moveWE = std::rand() %3 -1;
		tmp.x+=moveWE;
		tmp.y+=moveNS;
		if(( tmp.x >= 1 && tmp.x <= windowWidth - 2) && (tmp.y >= 1 && tmp.y <= windowHeight - 2))
			snakePosition.at(snakePosition.size()-1) = tmp;
	}
}

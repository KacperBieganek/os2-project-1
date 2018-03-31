#include "snake.h"
#include <thread>
#include <ctime>
#include <memory>

void makeWindows(std::vector<WINDOW*>& windowVector,int height,int width);
WINDOW* create_newwin(int height, int width, int starty, int startx);

int main(int argc,char** argv)
{
	int width,height;
	std::mutex mutex;
	volatile bool running  = true;
	std::vector<WINDOW*> windowVector;
	std::vector<std::shared_ptr<snake::Snake>> snakeVector;
	
	srand(time(NULL));
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_GREEN);
	init_pair(3,COLOR_WHITE,COLOR_BLUE);
	init_pair(4,COLOR_WHITE,COLOR_BLACK);
	getmaxyx(stdscr,height,width);
	refresh();
	
	makeWindows(windowVector,height,width);
	int windowCounter=0;
	for(auto win: windowVector)
	{
		snakeVector.push_back(std::make_shared<snake::Snake>(win,width/2,height/2,mutex,running));
		wbkgd(win,COLOR_PAIR(++windowCounter));
		wrefresh(win);
	}


	
	std::vector<std::thread> threads;
	for(auto snake : snakeVector)
	{
	threads.push_back(std::thread(&snake::Snake::run,snake));
	}
	
	
	
	std::this_thread::sleep_for(std::chrono::seconds(10));
	running = false;

	for(auto& thread : threads)
		thread.join();

	endwin();
	return 0;
}

void makeWindows(std::vector<WINDOW*>& windowVector, int height, int width)
{
	windowVector.push_back(create_newwin(height/2,width/2,0,0));
	windowVector.push_back(create_newwin(height/2,width/2,0,width/2));
	windowVector.push_back(create_newwin(height/2,width/2,height/2,0));
	windowVector.push_back(create_newwin(height/2,width/2,height/2,width/2));
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	
	WINDOW* local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, '*' , '*');
	wrefresh(local_win);
	return local_win;
}



	


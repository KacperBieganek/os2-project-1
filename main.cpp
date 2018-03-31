#include <vector>
#include <thread>
#include <mutex>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <unistd.h>

std::mutex mx;
volatile bool running = true;


struct Point{
	int x,y;
	Point(int x, int y): x(x),y(y) {};
};

void makeWindows(std::vector<WINDOW*>& windowVector,int height,int width);
WINDOW* create_newwin(int height, int width, int starty, int startx);
void initSnake(int height, int width, std::vector<Point>& points);
void drawSnake(WINDOW* win, std::vector<Point>& snakePosition);
void moveSnake(std::vector<Point>& snakePosition, int windowWidth, int windowHeight);
void runSnake(WINDOW* win, std::vector<Point>* snakePosition, int windowWidth, int windowHeight);

int main(int argc,char** argv)
{
	int width,height;
	std::vector<WINDOW*> windowVector;
	std::vector<Point> upperLeftSnake;
	std::vector<Point> upperRightSnake;
	std::vector<Point> bottomLeftSnake;
	std::vector<Point> bottomRightSnake;

	
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
		wbkgd(win,COLOR_PAIR(++windowCounter));
		wrefresh(win);
	}

	initSnake(height/2,width/2,upperLeftSnake);
	initSnake(height/2,width/2,upperRightSnake);
	initSnake(height/2,width/2,bottomLeftSnake);
	initSnake(height/2,width/2,bottomRightSnake);	

	std::vector<std::thread> threads;
	
	threads.push_back(std::thread(runSnake,windowVector[0],&upperLeftSnake,width/2,height/2));
	threads.push_back(std::thread(runSnake,windowVector[1],&upperRightSnake,width/2,height/2));
	threads.push_back(std::thread(runSnake,windowVector[2],&bottomLeftSnake,width/2,height/2));
	threads.push_back(std::thread(runSnake,windowVector[3],&bottomRightSnake,width/2,height/2));
	
	std::this_thread::sleep_for(std::chrono::seconds(10));
	::running = false;

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

void initSnake(int height, int width, std::vector<Point>& points)
{
	for(int i=0;i<5;i++)
	{
		points.push_back(Point(width/2+i, height/2));
	}
}

void drawSnake(WINDOW* win, std::vector<Point>& snakePosition)
{
	wclear(win);
	box(win,'*','*');
	for(auto point : snakePosition)
	{
		mvwprintw(win,point.y,point.x,"o");
	}
	wrefresh(win);
}


void moveSnake(std::vector<Point>& snakePosition, int windowWidth, int windowHeight)
{
	Point tmp = snakePosition.back();
	bool legalMove = false;
	std::rotate(snakePosition.begin(),snakePosition.begin()+1,snakePosition.end());
	
	int moveNS = std::rand() %3 -1;
	int moveWE = std::rand() %3 -1;
	
	tmp.x+=moveWE;
	tmp.y+=moveNS;

	if(( tmp.x >= 1 && tmp.x <= windowWidth - 2) && (tmp.y >= 1 && tmp.y <= windowHeight - 2))
		snakePosition.at(snakePosition.size()-1) = tmp;

}


void runSnake(WINDOW* win, std::vector<Point>* snakePosition, int windowWidth, int windowHeight)
{
	while(::running)
	{
	moveSnake(*snakePosition,windowWidth,windowHeight);	
	{
		std::lock_guard<std::mutex> lock(mx);
		drawSnake(win,*snakePosition);
	}
		usleep(75000);
	}
}
	


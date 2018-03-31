#include <vector>
#include <thread>
#include <mutex>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::mutex mx;
bool running = true;


struct Point{
	int x,y;
	Point(int x,int y): x(x),y(y) {};
};

void makeWindows(std::vector<WINDOW*>& windowVector,int height,int width);
WINDOW* create_newwin(int height, int width, int starty, int startx);
void initSnake(int height, int width, std::vector<Point>& points);
void drawSnake(WINDOW* win, std::vector<Point>& snakePosition);
void moveSnake(std::vector<Point>& snakePosition, int windowWidth, int windowHeight);

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
	int counter=0;
	for(auto win: windowVector)
	{
		wbkgd(win,COLOR_PAIR(++counter));
		wrefresh(win);
	}

	initSnake(height/2,width/2,upperLeftSnake);
	initSnake(height/2,width/2,upperRightSnake);
	initSnake(height/2,width/2,bottomLeftSnake);
	initSnake(height/2,width/2,bottomRightSnake);

	moveSnake(upperLeftSnake,width/2,height/2);

	drawSnake(windowVector[0],upperLeftSnake);
	drawSnake(windowVector[1],upperRightSnake);
	drawSnake(windowVector[2],bottomLeftSnake);
	drawSnake(windowVector[3],bottomRightSnake);
	
	getch();
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
		points.push_back(Point(height/2,width/2+i));
	}
}

void drawSnake(WINDOW* win, std::vector<Point>& snakePosition)
{
	wclear(win);
	box(win,'*','*');
	for(auto point : snakePosition)
	{
		mvwprintw(win,point.x,point.y,"o");
	}
	wrefresh(win);
}


void moveSnake(std::vector<Point>& snakePosition, int windowWidth, int windowHeight)
{
	Point tmp = snakePosition.back();
	bool legalMove = false;
	std::rotate(snakePosition.begin(),snakePosition.begin()+1,snakePosition.end());
	
	do
	{
	int moveNS = std::rand() %3 - 1;
	int moveWE = std::rand() %3 - 1;
	if(tmp.x+moveWE == 0 || tmp.x+moveWE == windowWidth - 1)
		continue;
	if(tmp.y+moveNS == 0 || tmp.y+moveNS == windowHeight - 1)
		continue;

	tmp.x+=moveWE;
	tmp.y+=moveNS;
	snakePosition.at(snakePosition.size()-1) = tmp;
	legalMove=true;
	}while(!legalMove);

}

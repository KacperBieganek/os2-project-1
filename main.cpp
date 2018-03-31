#include <vector>
#include <thread>
#include <mutex>
#include <ncurses.h>

std::mutex mx;


struct Point{
	int y,x;
};

void makeWindows(std::vector<WINDOW*>& windowVector,int height,int width);
WINDOW* create_newwin(int height, int width, int starty, int startx);
void initSnake(WINDOW* win,std::vector<Point>& points);

int main(int argc,char** argv)
{
	int width,height;
	std::vector<WINDOW*> windowVector;
	initscr();
	start_color();
	getmaxyx(stdscr,height,width);
	refresh();
	makeWindows(windowVector,width,height);
	//printw("height: %d  width: %d",height,width);
	getch();
	endwin();
	return 0;
}

void makeWindows(std::vector<WINDOW*>& windowVector, int width, int height)
{
	windowVector.push_back(create_newwin(height/2,width/2,0,0));
	windowVector.push_back(create_newwin(height/2,width/2,height/2,0));
	windowVector.push_back(create_newwin(height/2,width/2,0,width/2));
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


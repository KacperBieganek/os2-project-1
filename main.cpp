#include <vector>
#include <thread>
#include <mutex>
#include <ncurses.h>

int main(int argc,char** argv){
	int width,height;
	initscr();
	getmaxyx(stdscr,height,width);
	printw("height: %d  width: %d",height,width);
	refresh();
	getch();
	endwin();
	return 0;
}

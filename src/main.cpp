#include "engine.cpp"

int main()
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
	
	load();
	
	initscr();
	noecho();
	
	chunk[x][y].init();
	chunk[x][y].show();
	
	while(!toExit)
		chunk[x][y]();
	
	printw("testend");
	
	endwin();
	
	return 0;
}

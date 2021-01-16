/*	Nostrum Asci game engine
*
*	Copyright (C) 2018-2021  Paweł Musiał
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU Affero General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU Affero General Public License for more details.
*
*	You should have received a copy of the GNU Affero General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
****************************************************************************
*/

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

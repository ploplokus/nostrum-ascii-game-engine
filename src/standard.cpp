/*	Nostrum Ascii game engine
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

#ifndef standard_cpp
#define standard_cpp

#include <clocale>
#include <ncurses.h>
#include <string>
#include <cstdint>

using std::string;
using std::to_string;

#include "pl.cpp"
#include "mainGlobal.cpp"

enum PauseType
{
	normal,
	silent

};	//	enum PauseType 0O1Ili

uint16_t pause(PauseType pauseType)
{

	switch(pauseType)
	{
		case normal:

			printw("%s", Qpause.c_str() );
			refresh();
			getch();
			return 0;

		case silent:

			getch();
			return 0;

		default:

			return 1;
	}	//	swich(pauseType)

}

void clearScreen()
{
	clear();
	refresh();
}

void warn(string a)
{
	clearScreen();

	printw("%s", a.c_str());
	
	pause(normal);

}


enum choiceType
{
	Clear,
	interact
};	//	enum choiceType

uint16_t choosing(choiceType type, uint16_t I, string* texts, string description, string canelInteraction, uint16_t & start)
{
	uint16_t choice = 0;
	char a = 'q';

	if(I >= start)	//	zachowywanie poprzedniej pozycji wyboru
		choice = start;
	else
		choice = I;	//	jeśli poprzedni wybór był większy, to pozycja jest ostatnia

	if(I)
	{
		while( ( a != 'z' ) && (a != 'Z') )
		{
			clearScreen();
			
			printw("%s", description.c_str() );
			
			if(type == interact)
			{
				if(choice)
					printw("  %s", canelInteraction.c_str() );
				else
					printw("[ %s ] ", canelInteraction.c_str() );
				
				printw("\n");
			}

			for(uint16_t i = 0 ; i < I ; i++)	//	Wypisanie nazw możliwych interiakcji
			{
				if(i==(choice-1))
					printw("[ %u. %s ]\n", i+1, texts[i].c_str() );
				else
					printw("  %u. %s\n", i+1, texts[i].c_str() );
			}
			
			refresh();
			
			a = getch();	//	Wczytanie wyboru gracza ('z' jest zatwierdzeniem wyboru)

			switch(a)
			{

				case 'W':
				case 'w':

					if(choice)
					{	if( !( (choice==1 )&&( type == Clear) ) )
							choice--;
					}
				break;

				case 'S':
				case 's':

				if(choice<I)
				{
					choice++;
				}
				break;
			}
		}
	}else
	{
		clearScreen();
		choice = 0;
		printw("%s\n\n", description.c_str() );
		pause( silent );
	}

	return start = choice;
}

void setCursorXY( uint16_t x, uint16_t y )	//	zmiana położenia kursora
{
	move( y, x );
}
#endif	//	standard_cpp

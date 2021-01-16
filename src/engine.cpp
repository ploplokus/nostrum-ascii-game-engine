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

#include "standard.cpp"

#ifndef engine_cpp
#define engine_cpp

enum wasdNum
{
	WN,
	SN,
	DN,
	AN
};

void irqExe(uint16_t irq);

void goToPlace(uint16_t x, uint16_t y, wasdNum direction);

string bar = "$\n", interactionHistory = QhereIsAll;

uint16_t lastChoice = 0;

bool ifShowInventory = false, toExit = false;

char scrBuff[scrX][scrY];	//	bufor ekranu

//char inputChar = '!';

///	Do dokończenia i wprowadzenia, połączenia z resztą kodu. Idea do zmienienia
/*
class packedWorld
{
	uint16_t I;
	uint16_t* X, Y;
	Place* places;

	show()
	{
		for(uint16_t i = 0 ; i < I ; i++)
		{
			::tab[ X[i] ][ Y[i] ]=places[i];
		}
	}


	set(uint16_t nr)
	{
		I = nr;
		X = new uint16_t [I];
		Y = new uint16_t [I];
		places = new Place [I];
	}


	packedWorld()
	{
		Place = X = Y = i = 0;
	}

};

*/

/*class item
{
	uint16_t ID;
	itemType a;
};
*/

class Dialogue
{
	friend void load();
	uint16_t I, J;
	string* lines;
	string* names;
	uint16_t* wsk;

	public:
	
	void operator () (const string &placeAllReturn)
	{
		for( uint16_t i = 0 ; i < I ; i++ )
		{
			clearScreen();
			
			printw("%s", (placeAllReturn + interactionHistory + "\n").c_str() );
			
			if( ( wsk[i] < J ) && (names[ wsk[i] ] != "") )
				printw("%s", names[ wsk[i] ].c_str() );
			else
				printw("BBeezziimmiieennyy");
				
			printw("\n%s", lines[i].c_str() );
			
			refresh();
			pause( silent );
			
		}
	}
	
	string allReturn()
	{
		string ret = "";
		for(uint16_t i = 0 ; i < I ; i++)
		{
			ret += "\n";
			
			if( ( wsk[i] < J ) && (names[ wsk[i] ] != "") )
				ret += names[ wsk[i] ];
			else
				ret += "BBeezziimmiieennyy";
				
			ret += ( "\n" + lines[i] + "\n" );
		}
		
		return ret;
	}
	
	string lastReturn()
	{
		string ret = "";
		
		ret += "\n";
			
			if( ( wsk[I-1] < J ) && (names[ wsk[I-1] ] != "") )
				ret += names[ wsk[I-1] ];
			else
				ret += "BBeezziimmiieennyy";
				
			ret += ( "\n" + lines[I-1] + "\n" );
		
		return ret;
	}
	
	void Set(uint16_t nr, uint16_t nr2)
	{
			if(I)
			{
				delete [] lines;
				delete [] wsk;
			}
			if(J)
			delete [] names;
			
		
		I = nr;
		
		if (I)
		{
			lines = new string [I];
			wsk = new uint16_t [I];
		}
		
		J = nr2;
		
		if (J)
			names = new string [J];
	
	}
	
	Dialogue(uint16_t nr, uint16_t nr2)
	{
		lines = nullptr;
		wsk = nullptr;
		names = nullptr;
		
		Set(nr, nr2);
	}
	
	Dialogue()
	{
		I = J = 0;
		lines = nullptr;
		wsk = nullptr;
		names = nullptr;
	}
	
	~Dialogue()
	{
		delete [] lines;
		delete [] wsk;
		delete [] names;
	}
};

class Interaction	//	Możliwe wybory protagonisty prowadzące do kolejnych lub do końca interakcji
{
	friend void load();
	uint16_t I, irq;
	string* names;
	Interaction** wsk;
	string name;
	Dialogue desc;
	public:
	
	
	void operator () (const string &placeAllReturn )	//	Sama interiakcja
	{
		
		interactionHistory += (name + "\n" );
		printw("%s", interactionHistory.c_str() );
		
		if(irq)
			irqExe(irq);
		
		refresh();
		
		desc( placeAllReturn );
		
		interactionHistory += desc.lastReturn() + "\n";
		
		uint16_t choice = choicing( interact, I, names, placeAllReturn + interactionHistory , QcanelInteraction, lastChoice );
		
		clearScreen();
		interactionHistory = "";
		
		if( choice && ( ( choice - 1 ) < I ) )
			( *wsk[choice-1] )( placeAllReturn );
	}


	void Set(uint16_t nr, string n, uint16_t IRQ)
	{
			if(I)
		{
			delete [] wsk;
			delete[] names;
		}
		
		I = nr;
		
		if(I)
		{
			wsk = new Interaction* [I];
			names = new string[I];
		}
		
		name = n;
		irq = IRQ;
	}
	
	void buildNames()
	{
		for( uint16_t i = 0 ; i < I ; i++)
			names[i] = wsk[i]->name;
	}
	
	Interaction(uint16_t nr, string n, uint16_t IRQ)
	{
		wsk = nullptr;
		names = nullptr;
		
		Set(nr, n, IRQ);
	}
	
	Interaction()
	{
		I = irq = 0;
		name = Qvoid123;
		wsk = nullptr;
		names = nullptr;
	}
	
	~Interaction()
	{
		delete [] wsk;
		delete [] names;
	}
};

class Unit	//	Samodzielny, odrębny obiekt zdolny do interakcji
{
	friend void load();
	friend class Place;
	
	uint16_t I;	//	interactions
	string description, endInteraction;
	string name;
	Interaction* interactions;
	bool showName;
	char bgnd;	//	przechowuje tło za obiektem
	public:
	
	uint16_t x, y;
	char a;
	
	void hide()
	{
		setCursorXY(x, y);
		printw("%c", scrBuff[x][y]);
		refresh();
	}
	
	void operator () (const string &placeAllReturn )	//	interakcja z objektem
	{
		interactionHistory = "";
		clearScreen();
			
			if(showName)
				interactionHistory += name + "\n";
				
			interactionHistory += description;
			
		if(I)	
			interactions[0]( placeAllReturn );	//	Zawsze jedna i pierwsza interakcja
		
		else{
		
				printw("%s", (placeAllReturn + interactionHistory).c_str() );
				refresh();
				pause( silent );
				clearScreen();
				
			}
		
		
		printw("%s", (placeAllReturn + interactionHistory + endInteraction).c_str() );
		refresh();
		
		interactionHistory = "";
		pause( silent );
		clearScreen();
	}
	
	void Set(uint16_t X, uint16_t Y, char A, uint16_t nr, string n , string d, string e, bool isn)
	{
			if(I)
			delete [] interactions;
	
		x = X;
		y = Y;
		a = A;
		I = nr;
		
		if (I)
		{
			interactions = new Interaction[I];
		}
		
		name = n;
		description = d;
		endInteraction = e;
		showName = isn;
	}


	Unit()
	{
		name = description = endInteraction = Qvoid123;
		I = 0;
		interactions = nullptr;
		bgnd = 0;
		showName = true;
	}
	
	~Unit()
	{
		delete [] interactions;
	}
};

/*class chest : public Unit
{
	item** wsk;
	uint16_t I;
	void setItemTab(uint16_t nr)
	{
		I=nr;
		
		if(I)
			wsk = new item * [I];
	}

};*/

Unit protagonist;

uint16_t & refX = protagonist.x, & refY = protagonist.y;

struct GndChar	//	znak tła
{
	bool solid, unit;
	char a;
};

void showInventory()	//	Pokazanie ekwipunku
{
	uint16_t x = 3, y = 3;
	
	for(uint16_t i = 0 ; i < 17 ; i ++)
	{
		scrBuff[x][y] = '#';
		x++;
	}
	
	scrBuff[x][y] = '*';
	
	x = 3;
	y += 1;
	
	scrBuff[x][y] = '*';
	
	x++;
	
	for(uint16_t i = 0 ; i < 16 ; i++)
	{
		scrBuff[x][y] = Qinventory[i];
		x++;
	}
	scrBuff[x][y] = '*';
	
	x = 3;
	y += 1;
	
	for(uint16_t i = 0 ; i < 17 ; i ++)
	{
		scrBuff[x][y] = '#';
		x++;
	}
	scrBuff[x][y] = '*';
}

class Place	//	miejsce świata gry wyświetlane pojedyńczo, plansza
{
	friend void load();
	friend void goToPlace(uint16_t x, uint16_t y, wasdNum direction);
	
	string description;
	bool exitDirecton[4];	//	możliwe kierunki wyjścia odpowiednio północ, południe, wschód, zachód
	bool forceInteractionIn, forceInteractionOut;	//wymuszona interakcja przy wejściu i wyjściu z planszy
	uint16_t I;	//	liczba objektów w tablicy
	string name;
	Unit* units;
	GndChar bgnd[scrX][scrY];	//	tło
	public:
	
	uint16_t findUnit(uint16_t x, uint16_t y)	//	Wywowyłuje obiekt o podanych koordynatach
	{	
		for(uint16_t i = 0 ; i < I ; i++)
			if( ( units[i].x == x ) && ( units[i].y == y ) )
				return i;
			
		return 0;
	}
	
	void moveUnit( uint16_t upn, uint16_t x, uint16_t y)	//	upn - Unit Place chunk nubmer unitPlaceChunk
	{
		uint16_t tmpX = units[upn].x, tmpY = units[upn].y;
		
		if(units[upn].bgnd)
			bgnd[tmpX][tmpY].a = units[upn].bgnd;
		
		bgnd[tmpX][tmpY].unit = false;
		
		units[upn].x = x;
		units[upn].y = y;
		
		init();
	}
	
	void moveUnit(uint16_t upn, uint16_t x, uint16_t y, Place &dp)	//	upn - Unit Place chunk nubmer, this cp - current Place, dp - destination Place
		{
			if(this == &dp)
				moveUnit( upn, x, y);
			else
				warn(QUnitList);
		}
	
	void init()
	{
		//	buforowanie wyświetlania tła
		for(uint16_t i = 0 ; i < scrX ; i++)
			for (uint16_t j = 0 ; j < scrY ; j++)
				scrBuff[i][j] = bgnd[i][j].a;
		
		//	buforowanie wyświetlania Unit
		for( uint16_t i = 0 ; i < I ; i++ )
		{
			uint16_t tmpX = units[i].x, tmpY = units[i].y;
			
			if(units[i].a != bgnd[tmpX][tmpY].a)
				units[i].bgnd = bgnd[tmpX][tmpY].a;
				
			scrBuff[tmpX][tmpY] = units[i].a;
			bgnd[tmpX][tmpY].unit = true;
		}
		
		
		//	buforowanie wyświetlania ekwpunku
		if(ifShowInventory)
		{
			showInventory();
		}
	}


	void show()	//	właściwe wyświetlenie
	{
		setCursorXY(0, 0);	//	przejście na początek ekranu
		for(uint16_t i = 0 ; i < scrY ; i++)	//	nadpisanie planszy
		{
			for (uint16_t j = 0 ; j < scrX ; j++)
				printw("%c", scrBuff[j][i]);
				
			printw("\n");
		}
		
		refresh();
	}
	
	string allReturn()
	{
		string ret = "";
		
		protagonist.bgnd = scrBuff[refX][refY];
		scrBuff[refX][refY] = protagonist.a;
		
		for(uint16_t i = 0 ; i < scrY ; i++)	//	nadpisanie planszy
		{
			for (uint16_t j = 0 ; j < scrX ; j++)
				ret += scrBuff[j][i];
				
			ret+= "\n";
		}
		
		ret +=	"x=(" + to_string(refX) + ";" + to_string(refY) + ")\nP=(" + to_string(x) + ";" + to_string(y) + ")\n";
		
		scrBuff[refX][refY] = protagonist.bgnd;
		return ret;
	}


	void operator () ()	//	Pokazuje Place na ekranie i udostępnia ogólne sterowanie
	{	
		
		//	wyświetlanie potagonisty
		setCursorXY(refX, refY);
		printw("%c", protagonist.a);
		
		
		setCursorXY(0 , scrY );	//	przejście na początek paska
		
		//	pasek lokalizacji i innych statystyk oraz pasek główny (string bar)
		printw("x=(%u;%u)\nP=(%u;%u)\n%s", refX, refY, x, y, bar.c_str() );
		
		refresh();
		
		//char tmp = getch();	//	To były testy znaków wprowadzanych niestandardowo (np przez kółko myszy, strzałki)
		//inputChar = tmp;
		
		switch(getch())	//	obsługa sterowania
		{
			case 'W':
			case 'w':
				
				if( ( refY == 0 ) && exitDirecton[0] )
				{	
					if(forceInteractionOut && I)
						units[0](allReturn());
						
						goToPlace( x , y-1 , WN );
						
				}else if ( ( refY > 0 ) && bgnd[refX][refY - 1].unit)
					{
						units[ findUnit(refX , refY - 1) ]( allReturn() );
						show();
					
					}else if ( ( refY > 0 ) && bgnd[refX][refY - 1].solid)
						{
							bar += ( QYouAreGoingToWall + "\n" );
						
						}else
							{
								protagonist.hide();
								refY--;
							}
			
			break;
			
			case 'A':
			case 'a':
				
				if( ( refX == 0 ) && exitDirecton[3] )
				{
					if(forceInteractionOut && I)
						units[0](allReturn());
					
					goToPlace( x-1 , y , AN );
					
				}else if ( ( refX > 0 ) && bgnd[refX-1][refY].unit)
					{
						units[ findUnit(refX - 1 , refY) ]( allReturn() );
						show();
						
					}else if ( ( refX > 0 ) && bgnd[refX-1][refY].solid)
						{
							bar += ( QYouAreGoingToWall + "\n" );
						
						}else
							{
								protagonist.hide();
								refX--;
							}
							
			break;
			
			case 'S':
			case 's':
				
				if( ( refY >= scrY-1 ) && exitDirecton[1] )
				{
					if(forceInteractionOut && I)
						units[0](allReturn());
					
					goToPlace( x , y+1 , SN );
					
				}else if ( ( refY <= scrY ) && bgnd[refX][refY + 1].unit)
					{
						units[ findUnit(refX , refY + 1) ]( allReturn() );
						show();
					
					}else if ( ( refY <= scrY ) && bgnd[refX][refY + 1].solid)
						{
							bar += ( QYouAreGoingToWall + "\n" );
						
						}else
							{
								protagonist.hide();
								refY++;
							}
							
			break;
			
			case 'D':
			case 'd':
				
				if( ( refX >= scrX-1 ) && exitDirecton[2] )
				{
					if(forceInteractionOut && I)
						units[0](allReturn());
					
					goToPlace( x+1 , y , DN );
				
				}else if ( ( refX <= scrX ) && bgnd[refX + 1][refY].unit)
					{
						units[ findUnit(refX + 1 , refY) ]( allReturn() );
						show();
					
					}else if ( ( refX <= scrX ) && bgnd[refX + 1][refY].solid)
						{
							bar += ( QYouAreGoingToWall + "\n" );
					
						}	else
							{
								protagonist.hide();
								refX++;
							}
							
			break;
			
			case 'Q':
			case 'q':
				
				clearScreen();
				bar = " ~ ";
				init();
				show();
				
			break;
			
			case 'R':
			case 'r':
				
				init();
				show();
				
			break;
			
			case 'E':
			case 'e':
				
				if(ifShowInventory)
					ifShowInventory = false;
						else ifShowInventory = true;
						
							show();
							
			break;
			
			case 'P':
			case 'p':
				
				printw("\n\n%s\n\n", QthereIsNoPause.c_str() );
				
				pause( normal );
				toExit = true;
				
			break;
			
			case ':':
				
				printw(":");
				
				char input = ' ';
				string commands = "";
				do
				{	
					input = getch();
					
					if(input != '\n' && input != '\e' )
					{
						printw("%c", input);
						commands += input;
					}
					
				} while(input != '\n' && input != '\e' );
				
				clearScreen();
				init();
				show();
				
				if(input == '\n')
				
					for(uint16_t i = 0; i < commands.length(); i++)
					
						switch(commands[i])	//	obsługa komend
							{
								case 'q':
								
									toExit = true;
								
							}	//	switch(commands[i]) //	obsługa komend
			break;
			
		}	//	switch(getch())	//	obsługa sterowania
		
	}


	void Set(uint16_t nr, string nm, string d, bool n, bool s, bool e, bool w, bool fi, bool fo)
	{
			if(I)
			delete [] units;
		I = nr;
		
		if (I)
			units = new Unit [I];
			
		name = nm;
		description = d;
		exitDirecton[0] = n;
		exitDirecton[1] = s;
		exitDirecton[2] = e;
		exitDirecton[3] = w;
		forceInteractionIn = fi;
		forceInteractionOut = fo;
	}
	
	Place(uint16_t nr, string nm, string d, bool n, bool s, bool e, bool w, bool fi, bool fo)	
	{
		units = nullptr;
		Set(nr, nm, d, n, s, e, w, fi, fo);
	}
	
	Place()
	{
		name = description = Qvoid123;
		I = 0;
		units = nullptr;
		exitDirecton[0] = exitDirecton[1] = exitDirecton[2] = exitDirecton[3] =forceInteractionIn = forceInteractionOut = false;
	}
	
	~Place()
	{
		delete[] units;
	}
};

Place chunk[xMax][yMax];

void goToPlace(uint16_t x, uint16_t y, wasdNum direction)	//	Przemieszczanie się pomiędzy planszami
{
	switch(direction)
	{
		case WN:
			refY = (scrY-1);
		break;
		
		case SN:
			refY = 0;
		break;
		
		case DN:
			refX = 0;
		break;
		
		case AN:
			refX = (scrX-1);
		break;
	}
		if( (x < xMax) && (y < yMax) )
		{
			::x = x;
			::y = y;
			
			chunk[x][y].init();
			chunk[x][y].show();
			
			if(chunk[x][y].forceInteractionIn && chunk[x][y].I)
			{
				chunk[x][y].units[0]( chunk[x][y].allReturn() );
				
				chunk[x][y].show();
			}
			
		}
}


	#include "irq.cpp"
	#include "creating.cpp"	//	wypełnianie mapy	//	void load();

#endif	//	engine_cpp

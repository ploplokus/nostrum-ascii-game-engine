void load()
{

	Place* a;
	Unit* b;
	Interaction* c;
	Dialogue* d;
	
	//	wypełnianie plansz właściwościami i objektami
	a = &chunk[0][0];
	
	a->Set( 2 , "" , "1. cwiartka" , 0 , 1 , 1 , 0 , 1 , 0 );
	
	
		b = &a->units[0];
	
		b->Set( 1 , 1 , 'R' , 2 , "Robot", "Jestem Robot 2187\n\n", "Cyberman?", true);
	
			c=&b->interactions[0];
	
			c->Set( 1, "Powiedz coś", 1);
	
				d=&c->desc;
	
				d->Set(2, 1);
				d->names[0] = "Robot";
	
				d->wsk[0] = 0;
				d->lines[0] = "coś";
	
				d->wsk[1] = 0;
				d->lines[1] = "i coś";
	
	
			c=&b->interactions[1];
	
			c->Set( 1, "Powiedz cos innego", 2);
	
				d=&c->desc;
	
				d->Set(2, 1);
				d->names[0] = "Robot";
	
				d->wsk[0] = 0;
				d->lines[0] = "coś";
	
				d->wsk[1] = 0;
				d->lines[1] = "i coś, czyli 7812";
	
	
			b->interactions[0].wsk[0] = &b->interactions[1];
			b->interactions[1].wsk[0] = &b->interactions[0];
	
			b->interactions[0].buildNames();
			b->interactions[1].buildNames();
	
	
		a->units[1].Set( 10 , 10 , 'S' , 0 , "Serwer", "Jestem Serwer Gry\n\n", "Proszę mi nie przeszkadzać", true);
	
		a->init();
	
	a = &chunk[0][1];
	
	a->Set( 1 , "" , "2. cwiartka" , 1 , 0 , 1 , 0 , 0 , 1 );
	
		a->units[0].Set( 3 , 8 ,' ', 0 , "Szpiig", "Jak mnie znalazłes?\n\n", "Nieważne, i tak nie jestem Ważnym szpiegiem", true );
	
		a->init();
	
	chunk[1][0].Set( 0 , "" , "3. cwiartka" , 0 , 1 , 0 , 1 , 0 , 0 );
	
	
	
	a = &chunk[1][1];
	
	a->Set( 1 , "" , "4. cwiartka" , 1 , 0 , 0 , 1 , 0 , 0 );
	
	
		a->units[0].Set( 0 , 2 , '#' , 0 ,"<Nazwa gry>", "Kiedy <Nazwa gry>?\n\n", "Zapytaj Chucka Norrisa", true );
		
		a->init();
	
	//	Przybycie prptoagonisty
	protagonist.Set(10,10,'X',0,"Protagonista?","","", 0);
	
	
	//	Tworzenie obramowania i środka plansz
	for(uint16_t a = 0 ; a < xMax ; a++)
	{
		for(uint16_t b = 0 ; b < yMax ; b++)
		{
			for(uint16_t i = 0 ; i < scrX ; i++)
			{
				for(uint16_t j = 0 ; j < scrY; j++)
				{
	
					if( (i==0 ) || ( j==0 ) || (i== (scrX-1)) || (j== (scrY-1)) )
					{
						chunk[a][b].bgnd[i][j].solid = 1;
						chunk[a][b].bgnd[i][j].a='#';
					}else
					{
						chunk[a][b].bgnd[i][j].solid = 0;
						chunk[a][b].bgnd[i][j].a=' ';
					}
				}
	
			}
		}
	}
	
	//	Tworzenie przejść między planszami
	
	a = &chunk[0][0];
	a->bgnd[10][scrY-1].solid = 0;
	a->bgnd[10][scrY-1].a=' ';
	a->bgnd[scrX-1][10].solid = 0;
	a->bgnd[scrX-1][10].a=' ';
	
	
	a = &chunk[1][0];
	a->bgnd[10][scrY-1].solid = 0;
	a->bgnd[10][scrY-1].a=' ';
	a->bgnd[0][10].solid = 0;
	a->bgnd[0][10].a=' ';
	
	a = &chunk[0][1];
	a->bgnd[10][0].solid = 0;
	a->bgnd[10][0].a=' ';
	a->bgnd[scrX-1][10].solid = 0;
	a->bgnd[scrX-1][10].a=' ';
	
	a = &chunk[1][1];
	a->bgnd[10][0].solid = 0;
	a->bgnd[10][0].a=' ';
	a->bgnd[0][10].solid = 0;
	a->bgnd[0][10].a=' ';
}

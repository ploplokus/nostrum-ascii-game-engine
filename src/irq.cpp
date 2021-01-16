void irqExe(uint16_t irq)
{
	
	switch(irq)
	{
		case 1:

			bar += "Zdarzenie pierwsze \n";
			
			chunk[0][0].moveUnit(1, 2, 6);	//	Serwer gry na (2;6)
			
		break;

		default:
			bar += "Nieznane zdarzenie \n";
	}
}

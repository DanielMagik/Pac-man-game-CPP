#include "Gra.h"
#include"Poziom.h"

Gra::Gra()
{
	window.setFramerateLimit(60); //ustawienie 60 fps�w
	okno = GetConsoleWindow();
	ShowWindow(okno, SW_HIDE);//ukrycie konsoli
}
void Gra::graj()
{
	bool poczatek = true;
	while (true)//p�tla gry
	{
		try
		{
			//menu jest uruchamiane tylko raz (na pocz�tku gry) 
			if (uruchom_menu == true)
			{
				window.clear();
				menu.main_menu(&window);
			}
			//je�li menu by�o ju� raz uruchamiane, nie jest uruchamiane ponownie
			else
			{
				Co_dalej co_dalej = gra;
				throw co_dalej;
			}
		}
		catch (Co_dalej co_dalej)
		{
			uruchom_menu = false;//menu nie b�dzie uruchomione ponownie
			if (co_dalej == wyjscie)//wyj�cie z gry
			{
				bool b = false;
				throw b;
			}
			if (co_dalej == gra && poczatek)//reset zapisu
			{
				reset();
			}
			if (co_dalej == gra || co_dalej == kontynuuj_gre)//gra
			{
				poczatek = false;
				try
				{
					poziom.wczytaj();
					//uruchomienie poziomu
					poziom.uruchom(&window);
				}
				catch(Po_poziomie p)
				{
					//co robi� po zako�czeniu poziomu
					if (p == wyjscie_z_gry)
					{
						bool b = false;
						throw b;
					}
					//modyfikacja pliku zapisu
					if (p == koniec_levela)
					{
						poziom.obecny_level++;
						fstream plik;
						plik.open("PLIKI/zapis.txt", ios::out);
						plik << poziom.obecny_level << endl;//level
						plik << poziom.punkty << endl;//punkty
						plik << poziom.zycia << endl;//�ycia
						plik << "//level" << endl << "//punkty" << endl << "//zycia" << endl;//komentarz
						plik.close();
						poziom.pacman.zeruj_predkosc();
					}
					if (p == reset_levela)
					{
						poziom.restart_poziomu();
					}
				}
			}
			//kontynuacja wcze�niejszej gry
			
		}
	}
}
//modyfikacja pliku zapami�tuj�cego zapis gry
void Gra::reset()
{
	fstream plik;
	plik.open("PLIKI/zapis.txt", ios::out);
	plik << 1 << endl;//level
	plik << 0 << endl;//punkty
	plik << 100 << endl;//�ycia
	plik << "//level" << endl << "//punkty" << endl << "//zycia" << endl;//komentarz
	plik.close();
	poziom.zeruj_punkty();
}


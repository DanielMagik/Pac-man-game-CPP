#include "Duszek.h"

Duszek::Duszek()
{
	grafika.setOrigin(10, 10);
}
//pobranie liczby wierzcho�k�w z wy�szej klasy graf
void Duszek::pobierz_liczbe_wierzcholkow(int liczba)
{
	liczba_wierzcholkow = liczba;
}
//znalezienie wierzcho�ka jeszcze nieprzetworzonego z najmniejsz� warto�ci� w tablicy
//dystans. Wykorzystywane w algorytmie Dijkstry
int Duszek:: znajdz_najmniejszy_nieobsluzony(int meta)
{
	int indeks = -1;//warto��, gdy nie istnieje wierzcho�ek do obs�u�enia
	int minimum = tab_dystans[meta];
	//przetworzenie wszystkich wierzcho�k�w
	for (int i = 0; i < liczba_wierzcholkow; i++)
	{
		if (tab_przetworzono[i] == false && tab_dystans[i] < minimum)
		{
			minimum = tab_dystans[i];
			indeks = i;
		}
	}
	if (indeks != -1)
	{
		tab_przetworzono[indeks] = true;
	}
	return indeks;
}
//sprawdzenie, czy grafika duszka wysz�a poza plansz� i ewnetualna
//korekta wsp�rz�dnych
void Duszek::duszek_przekroczyl_tunel()
{
	//duszek wyszed� za bardzo w lewo
	if (grafika.getPosition().x < -18)
	{
		grafika.setPosition(542, grafika.getPosition().y);
	}
	else
		//duszek wyszed� za bardzo w prawo
		if (grafika.getPosition().x > 542)
		{
			grafika.setPosition(-18, grafika.getPosition().y);
		}

	//duszek wyszed� za bardzo w g�r�
	if (grafika.getPosition().y < -18)
	{
		grafika.setPosition(grafika.getPosition().x, 596.625);
	}
	else
		//duszek wyszed� za bardzo w d�
		if (grafika.getPosition().y > 596.625)
		{
			grafika.setPosition(grafika.getPosition().x, -18);
		}
}
//sprawdzenie, czy duszek jest na wierzcho�ku grafu, co za tym idzie, czy wylicza� drog�
bool Duszek::czy_duszek_jest_na_wierzcholku(int tablica_ID[31][28])
{
		int px1, py1;
		//ustalenie pola tablicy w kt�rym jest �rodek duszka
		px1 = int(grafika.getPosition().x);
		py1 = int(grafika.getPosition().y);
		int tablicax = int(px1 / 19), tablicay = int(py1 / 19);
		//�rodek duszka jest w �rodku pola
		if (px1 % 19 == 10 && py1 % 19 == 10)
		{
			int *wskaznik = tablica_ID[0];
			//odpowiednie przesuni�cie wska�nika
			wskaznik += 27 * tablicay + tablicay  + tablicax;
			
			if (*wskaznik != -1)
			{
				py = py1;
				px = px1;
				obecny_wierzcholek = *wskaznik;
				return true;
			}
		}
	return false;
}
//inicjalizowanie tablic i listy do algorytmu Dijkstry
void Duszek::uzupelnij_duszka(list<list<wierzcholek>> *lista)
{
	tab_wierzcholkow.reset(new wierzcholek*[liczba_wierzcholkow]);
	tab_dystans.reset(new int[liczba_wierzcholkow]);
	tab_poprzednik.reset(new wierzcholek*[liczba_wierzcholkow]);
	tab_przetworzono.reset(new bool[liczba_wierzcholkow]);
	for (int i = 0; i < liczba_wierzcholkow; i++)
	{
		tab_poprzednik[i] = nullptr;
		tab_dystans[i] = nieskonczonosc;
		tab_przetworzono[i] = false;
	}
	int iteracja = 0;
	for (list<list<wierzcholek>>::iterator it = (*lista).begin(); it != (*lista).end(); it++)
	{
		tab_wierzcholkow[iteracja] = &(*(it->begin()));//wska�niki na wierzcho�ki
		iteracja++;
	}
}
//wyznaczanie drogi od wierzcho�ka start do meta
void Duszek::dijkstra(int start, int meta, list <list <wierzcholek>> *lista)
{
	if (start != meta)
	{
		/////////////////////////////////////////RESET TABLIC
		for (int i = 0; i < liczba_wierzcholkow; i++)
		{
			if (tab_wierzcholkow[i]->ID == start)
			{
				tab_dystans[i] = 0;
			}
			else
			{
				tab_dystans[i] = nieskonczonosc;
			}
			tab_przetworzono[i] = false;
		}
		int najmniejszy_indeks;
		for (int i = 0; i < liczba_wierzcholkow; i++)
		{
			najmniejszy_indeks = znajdz_najmniejszy_nieobsluzony(meta);
			if (najmniejszy_indeks == -1)//brak wierzcho�k�w do obs�u�enia
			{
				break;
			}
			else
			{
				list <list <wierzcholek>>::iterator it;//znajdujemy wierzcho�ek w li�cie
				list<wierzcholek>::iterator sasiad;
				for (it = (*lista).begin(); it != (*lista).end(); it++)
				{
					if (it->begin()->ID == najmniejszy_indeks)//znaleziono wierzcho�ek
					{
						sasiad = it->begin();
						sasiad++;
						while (sasiad != it->end())
						{
							//odpowiednia zamiana, je�li znaleziono kr�tsz� drog� do danego wierzcho�ka
							if (tab_dystans[najmniejszy_indeks] + sasiad->odstep < tab_dystans[sasiad->ID])
							{
								tab_dystans[sasiad->ID] = tab_dystans[najmniejszy_indeks] + sasiad->odstep;
								tab_poprzednik[sasiad->ID] = &(*it->begin());
							}
							sasiad++;
						}
						break;
					}	
				}
			}
		}
	}
}
//tworzenie drogi na podstawie wcze�niej wykonanego algorytmu Dijkstry
void Duszek::stworz_droge(int start, int meta)
{
		//reset drogi
		droga.clear();
		dlugosc_drogi = 0;
		int obecny_indeks = meta;
		while (1)
		{
			wierzcholek w;
			w.ID = obecny_indeks;
			w.y = tab_wierzcholkow[obecny_indeks]->y;
			w.x = tab_wierzcholkow[obecny_indeks]->x;
			w.odstep = tab_dystans[obecny_indeks];//potrzebne do obs�ugi tunelu
			droga.push_front(w);
			dlugosc_drogi++;
			//je�li p�tla dosz�a do startu, to si� ko�czy
			if (obecny_indeks == start)
			{
				break;
			}
			else
			{
				obecny_indeks = tab_poprzednik[obecny_indeks]->ID;
			}
		}
		
}
//ustalenie pr�dko�ci na podstawie drogi
void Duszek :: ustal_kierunek(int y_pacmana, int x_pacmana, int ID_ducha)
{
	predkosc.x = 0;
	predkosc.y = 0;
	if (dlugosc_drogi>1)//duszek nie jest przy najblizszym wierzcholku pacmana
	{
		list<wierzcholek>::iterator it = droga.begin();
		int xpoczatkowe, ypoczatkowe, xkoncowe, ykoncowe;
		xpoczatkowe = it->x;
		ypoczatkowe = it->y;
		it++;
		xkoncowe = it->x;
		ykoncowe = it->y;
		int dlugosc = it->odstep;
		if (ypoczatkowe == ykoncowe)//ruch wzd�u� p�aszczyzny x
		{
			if (xpoczatkowe < xkoncowe)//ruch w prawo
			{
				predkosc.x = 1;
			}
			else//ruch w lewo
			{
				predkosc.x = -1;
			}
		}
		else//ruch wzd�u� p�aszczyzny y
		{
			if (ypoczatkowe < ykoncowe)//ruch w dol
			{
				predkosc.y = 1;

			}
			else//ruch w g�re
			{
				predkosc.y = -1;
			}
		}
		//tunel poziomy w lewo
		if (xpoczatkowe == 1 && xkoncowe == 26 && dlugosc==3)
		{
			predkosc.x = -1;
		}
		else//tunel poziomy w prawo
		if (xpoczatkowe == 26 && xkoncowe == 1 && dlugosc==3)
		{
			predkosc.x = 1;
		}
		else
		if (ypoczatkowe == 1 && ykoncowe == 29 && dlugosc == 3)
		{
			predkosc.y = -1;
		}
		else
		if (ypoczatkowe == 29 && ykoncowe == 1 && dlugosc == 3)
		{
			predkosc.y = 1;
		}
	}
	else//duszek jest blisko pacmana
	{
		if (ID_ducha == 2 || ID_ducha == 3)//pinky i inky si� nie ruszaj�
		{
			predkosc.x = 0;
			predkosc.y = 0;
		}
		else//rech w stron� pacmana
		{
			int y_duszka = int(grafika.getPosition().y / 19);
			int x_duszka = int(grafika.getPosition().x / 19);
			if (y_duszka == y_pacmana)//ruch wzd�u� p�aszczyzny x
			{
				if (x_duszka < x_pacmana)//ruch w prawo
				{
					predkosc.x = 1;
				}
				else//ruch w lewo
				{
					predkosc.x = -1;
				}
			}
			else//ruch wzd�u� p�aszczyzny y
			{
				if (y_duszka < y_pacmana)//ruch w dol
				{
					predkosc.y = 1;

				}
				else//ruch w g�re
				{
					predkosc.y = -1;
				}
			}
		}
	}
}
//wykonanie pojedy�czego ruchu
void Duszek::wykonaj_ruch()
{
	//2.375
	if (predkosc.x == -1 )
	{
		grafika.move(-2.375, 0);
		//grafika.move(-1, 0);

	}
	else
	if (predkosc.x == 1)
	{
		grafika.move(2.375, 0);
		//grafika.move(1, 0);
	}
	else
	if (predkosc.y == -1)
	{
		grafika.move(0, -2.375);
		//grafika.move(0, -1);
	}
	else
	if (predkosc.y == 1)
	{
		grafika.move(0, 2.375);
		//grafika.move(0, 1);
	}
}
//przemieszczenie grafiki duszka do domku
void Duszek::teleport_do_domku(int poczatkowe_y, int poczatkowe_x)
{
	grafika.setPosition(poczatkowe_x * 19 + grafika.getOrigin().x, poczatkowe_y * 19 + grafika.getOrigin().y);
	predkosc.x = 0;
	predkosc.y = 0;
}

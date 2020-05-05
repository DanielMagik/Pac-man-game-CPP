#include "graf.h"

Graf::Graf()
{
	//³adowanie tekstur
	blinky.tekstura.loadFromFile("GRAFIKA/blinky.png");
	pinky.tekstura.loadFromFile("GRAFIKA/pinky.png");
	inky.tekstura.loadFromFile("GRAFIKA/inky.png");
	clyde.tekstura.loadFromFile("GRAFIKA/clyde.png");
	blinky.grafika.setTexture(blinky.tekstura);
	pinky.grafika.setTexture(pinky.tekstura);
	inky.grafika.setTexture(inky.tekstura);
	clyde.grafika.setTexture(clyde.tekstura);
	for (int i = 1; i < 30; i++)
	{
		for (int j = 1; j < 27; j++)
		{
			tablica_ID[i][j] = -1;
		}
	}
}
//skopiowanie tablicy z wy¿szej klasy Poziom
void Graf::skopiuj_tablice(short int *tablica)//kopiowanie tablicy
{
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			tablica2[i][j] = *tablica;
			tablica++;
		}
	}
}
//tworzenie tablicy na podstawie tablicy2 do ³atwiejszej analizy
void Graf::stworz_tablice()
{
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			if (tablica2[i][j] == 1)
			{
				tablica[i][j] = 0;//duze kropki na mozliwa droge
			}
			else//pusta przestrzen lub miejsce w domku na droge
				if (tablica2[i][j] == 2 || tablica2[i][j] == 6)
				{
					tablica[i][j] = 0;
				}
				else//wierzcho³ek w domu na wierzcho³ek
					if (tablica2[i][j] == 4)
					{
						tablica[i][j] = 1;
					}
					else//wierzcho³ek na zewn¹trz na wierzcho³ek
						if (tablica2[i][j] == 5)
						{
							poczatek.y = i;
							poczatek.x = j;
							y_domku = i;
							x_domku = j;
							tablica[i][j] = 1;
							blinky.grafika.setPosition(poczatek.x * 19 + blinky.grafika.getOrigin().x, poczatek.y * 19 + blinky.grafika.getOrigin().y);
							pinky.grafika.setPosition(poczatek.x * 19 + pinky.grafika.getOrigin().x, poczatek.y * 19 + pinky.grafika.getOrigin().y);
							inky.grafika.setPosition(poczatek.x * 19 + inky.grafika.getOrigin().x, poczatek.y * 19 + inky.grafika.getOrigin().y);
							clyde.grafika.setPosition(poczatek.x * 19 + clyde.grafika.getOrigin().x, poczatek.y * 19 + clyde.grafika.getOrigin().y);
							blinky.px = poczatek.x;
							blinky.py = poczatek.y;
							pinky.px = poczatek.x;
							pinky.py = poczatek.y;
							inky.px = poczatek.x;
							inky.py = poczatek.y;
							clyde.px = poczatek.x;
							clyde.py = poczatek.y;
						}
						else//sciany ,droga i tunel zostaj¹
						{
							tablica[i][j] = tablica2[i][j];
						}
		}
	}
}
//zaznaczanie wierzcho³ków w tablicy ID
void Graf::zaznacz_wierzcholki()
{
	//analiza pól i ich s¹siadów
	for (int i = 1; i < 30; i++)
	{
		for (int j = 1; j < 27; j++)
		{
			if (tablica[i][j] == 0)
			{
				if (tablica[i][j - 1] <= 1 && tablica[i - 1][j] <= 1) // 0
				{													  //00
					tablica[i][j] = 1;
					tablica_ID[i][j] = liczba_wierzcholkow;
					liczba_wierzcholkow++;
				}
				else
					if (tablica[i - 1][j] <= 1 && tablica[i][j + 1] <= 1)//0
					{											         //00
						tablica[i][j] = 1;
						tablica_ID[i][j] = liczba_wierzcholkow;
						liczba_wierzcholkow++;
					}
					else
						if (tablica[i + 1][j] <= 1 && tablica[i][j + 1] <= 1)//00
						{											         //0
							tablica[i][j] = 1;
							tablica_ID[i][j] = liczba_wierzcholkow;
							liczba_wierzcholkow++;
						}
						else
							if (tablica[i + 1][j] <= 1 && tablica[i][j - 1] <= 1)  //00
							{												       // 0
								tablica[i][j] = 1;
								tablica_ID[i][j] = liczba_wierzcholkow;
								liczba_wierzcholkow++;
							}
			}
			else
				if (tablica[i][j] == 1)
				{
					tablica_ID[i][j] = liczba_wierzcholkow;
					liczba_wierzcholkow++;
				}
		}
	}
	blinky.pobierz_liczbe_wierzcholkow(liczba_wierzcholkow);
	pinky.pobierz_liczbe_wierzcholkow(liczba_wierzcholkow);
	inky.pobierz_liczbe_wierzcholkow(liczba_wierzcholkow);
	clyde.pobierz_liczbe_wierzcholkow(liczba_wierzcholkow);
}
//uzupe³nianie listy s¹siedztwa (tworzenie grafu)
void Graf ::uzupelnij_liste()
{
	bool czy_poczatek = false;//czy wierzcho³ek obecny to start dla duszków
	for (int i = 1; i < 30; i++)
	{
		for (int j = 1; j < 27; j++)
		{
			if (tablica[i][j] == 1)//znaleziono wierzcho³ek
			{
				if (i == poczatek.y && j == poczatek.x)
				{
					czy_poczatek = true;
				}
				list <wierzcholek>  podlista;//stworzenie listy
				wierzcholek nowy;
				nowy.y = i;
				nowy.x = j;
				nowy.ID = tablica_ID[i][j];
				podlista.push_back(nowy);
				//dodanie ewentualnych wierzcho³ków do listy
				if (tablica[i + 1][j] <= 1)
				{
					int nowyy = i + 1, nowyx = j;
					if (tablica[nowyy][nowyx] == 0)
					{
						while (tablica[nowyy][nowyx] != 1)
						{
							nowyy += 1;
						}
					}
					wierzcholek nowy1;
					nowy1.y = nowyy;
					nowy1.x = nowyx;
					nowy1.ID = tablica_ID[nowyy][nowyx];
					nowy1.odstep = nowyy - i;
					podlista.push_back(nowy1);
				}
				if (tablica[i - 1][j] <= 1)
				{
					int nowyy = i - 1, nowyx = j;
					if (tablica[nowyy][nowyx] == 0)
					{
						while (tablica[nowyy][nowyx] != 1)
						{
							nowyy -= 1;
						}
					}
					wierzcholek nowy1;
					nowy1.y = nowyy;
					nowy1.x = nowyx;
					nowy1.ID = tablica_ID[nowyy][nowyx];
					nowy1.odstep = i - nowyy;
					podlista.push_back(nowy1);
				}
				if (tablica[i][j + 1] <= 1)
				{
					int nowyy = i, nowyx = j + 1;
					if (tablica[nowyy][nowyx] == 0)
					{
						while (tablica[nowyy][nowyx] != 1)
						{
							nowyx += 1;
						}
					}
					wierzcholek nowy1;
					nowy1.y = nowyy;
					nowy1.x = nowyx;
					nowy1.ID = tablica_ID[nowyy][nowyx];
					nowy1.odstep = nowyx - j;
					podlista.push_back(nowy1);
				}
				if (tablica[i][j - 1] <= 1)
				{
					int nowyy = i, nowyx = j - 1;
					if (tablica[nowyy][nowyx] == 0)
					{
						while (tablica[nowyy][nowyx] != 1)
						{
							nowyx -= 1;
						}
					}
					wierzcholek nowy1;
					nowy1.y = nowyy;
					nowy1.x = nowyx;
					nowy1.ID = tablica_ID[nowyy][nowyx];
					nowy1.odstep = j - nowyx;
					podlista.push_back(nowy1);
				}



				//obs³uga tunelu

				if (i == 29 && tablica[i + 1][j] == 3)
				{
					wierzcholek nowy1;
					nowy1.y = 1;
					nowy1.x = j;
					nowy1.odstep = 3;
					nowy1.ID = tablica_ID[nowy1.y][nowy1.x];
					podlista.push_back(nowy1);
				}
				if (i == 1 && tablica[i - 1][j] == 3)
				{
					wierzcholek nowy1;
					nowy1.y = 29;
					nowy1.x = j;
					nowy1.odstep = 3;
					nowy1.ID = tablica_ID[nowy1.y][nowy1.x];
					podlista.push_back(nowy1);
				}

				if (j == 26 && tablica[i][j + 1] == 3)
				{
					wierzcholek nowy1;
					nowy1.y = i;
					nowy1.x = 1;
					nowy1.odstep = 3;
					nowy1.ID = tablica_ID[nowy1.y][nowy1.x];
					podlista.push_back(nowy1);
				}

				if (j == 1 && tablica[i][j - 1] == 3)
				{
					wierzcholek nowy1;
					nowy1.y = i;
					nowy1.x = 26;
					nowy1.odstep = 3;
					nowy1.ID = tablica_ID[nowy1.y][nowy1.x];
					podlista.push_back(nowy1);
				}
				if (czy_poczatek)
				{
					poczatek.ID = tablica_ID[i][j];
					czy_poczatek = false;
				}
				lista.push_back(podlista);
			}
		}
	}
	blinky.uzupelnij_duszka(&lista);
	pinky.uzupelnij_duszka(&lista);
	inky.uzupelnij_duszka(&lista);
	clyde.uzupelnij_duszka(&lista);
}
//reset tablicy ID i grafu
void Graf::zresetuj()
{
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			tablica_ID[i][j] = -1;
		}
	}
	lista.clear();
	liczba_wierzcholkow = 0;
}
//wyœwietlanie grafik duszków
void Graf::wyswietl(RenderWindow * okno)
{
	okno->draw(blinky.grafika);
	okno->draw(pinky.grafika);
	okno->draw(inky.grafika);
	okno->draw(clyde.grafika);
}
//znajdowanie najbli¿szego wierzcho³ka dla (y,x)
int Graf::znajdz_najblizszy_wierzcholek(int y, int x)
{
	wierzcholek w;
	if (tablica[y][x] == 1)//wierzcho³ek
	{
		w.y = y;
		w.x = x;
	}
	else
		if (tablica[y][x] == 3)//tunel
		{
			if (y == 0)
			{
				w.y = y + 1;
				w.x = x;
			}
			else
				if (y == 30)
				{
					w.y = y - 1;
					w.x = x;
				}
			if (x == 0)
			{
				w.y = y;
				w.x = x + 1;
			}
			else
				if (x == 27)
				{
					w.y = y;
					w.x = x - 1;
				}
		}
		else
			if (tablica[y][x] == 0)
			{

				bool lewo = true, prawo = true, dol = true, gora = true;
				///////////////////////////////////////////////////////////
				if (tablica[y + 1][x] > 1)
				{
					dol = false;
				}
				if (tablica[y - 1][x] > 1)
				{
					gora = false;
				}
				if (tablica[y][x + 1] > 1)
				{
					prawo = false;
				}
				if (tablica[y][x - 1] > 1)
				{
					lewo = false;
				}
				//////////////////////////////////////////////////////////
				for (int i = 1; i > 0; i++)
				{
					if (dol && tablica[y + i][x] == 1)
					{
						w.y = y + i;
						w.x = x;
						//return w;
						break;
					}

					if (gora && tablica[y - i][x] == 1)
					{
						w.y = y - i;
						w.x = x;
						//return w;
						break;
					}
					if (prawo && tablica[y][x + i] == 1)
					{
						w.y = y;
						w.x = x + i;
						//return w;
						break;
					}
					if (lewo && tablica[y][x - i] == 1)
					{
						w.y = y;
						w.x = x - i;
						//return w;
						break;
					}

				}

			}
	return tablica_ID[w.y][w.x];
}
void Graf::pobierz_wspolrzedne_pacmana(Pacman pacman)
{
	x_pacmana = pacman.px;
	y_pacmana = pacman.py;
	
}
//wyznaczanie drogi i ruch duszków
void Graf::logika_duszkow(Pacman pacman)
{

	//Blinky
	if (blinky_zyje)
	{
		blinky.duszek_przekroczyl_tunel();
		if (blinky.czy_duszek_jest_na_wierzcholku(tablica_ID))
		{
			//bezpoœrednio do najbli¿szego wierzczh³ka od pacmana
			najblizszy_wierzcholek_pacmana = znajdz_najblizszy_wierzcholek(y_pacmana, x_pacmana);
			blinky.dijkstra(blinky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana, &lista);
			blinky.stworz_droge(blinky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana);
			blinky.ustal_kierunek(y_pacmana, x_pacmana, 1);
		}
		blinky.wykonaj_ruch();
	}
	//////////////////////////////////////////////////
	//Clyde
	if (clyde_zyje)
	{
		clyde.duszek_przekroczyl_tunel();
		if (clyde.czy_duszek_jest_na_wierzcholku(tablica_ID))
		{
			int y_clyde = int(clyde.grafika.getPosition().y / 19);
			int x_clyde = int(clyde.grafika.getPosition().x / 19);
			//jeœli dystans od pacmana mniejszy ni¿ 8
			if (sqrt(pow((y_pacmana - y_clyde), 2) + pow((x_pacmana - x_clyde), 2)) < 8)
			{
				najblizszy_wierzcholek_pacmana = 0;
			}
			clyde.dijkstra(clyde.obecny_wierzcholek, najblizszy_wierzcholek_pacmana, &lista);
			clyde.stworz_droge(clyde.obecny_wierzcholek, najblizszy_wierzcholek_pacmana);
			clyde.ustal_kierunek(y_pacmana, x_pacmana, 2);
		}
		clyde.wykonaj_ruch();
	}
	////////////////////////////////////////
	//Pinky
	if (pinky_zyje)
	{
		pinky.duszek_przekroczyl_tunel();
		if (pinky.czy_duszek_jest_na_wierzcholku(tablica_ID))
		{
			int plusy = 0, plusx = 0;
			int pinky_y = y_pacmana, pinky_x = x_pacmana;
			if (pacman.predkosc.x == -1)
			{
				plusx = -1;
			}
			else
				if (pacman.predkosc.x == 1)
				{
					plusx = 1;
				}
				else
					if (pacman.predkosc.y == -1)
					{
						plusy = -1;
					}
					else
						if (pacman.predkosc.y == 1)
						{
							plusy = 1;
						}

			for (int i = 4; i >= 0; i--)//sprawdzamy 4,3,2,1 ewemtualnie 0 pole od pacmana
			{
				if ((pinky_y + plusy * i) > 0 && (pinky_y + plusy * i) < 30
					&& (pinky_x + plusx * i > 0) && (pinky_x + plusx * i) < 27)//czy nie wychodzimy poza tablice
				{
					if (tablica[pinky_y + plusy * i][pinky_x + plusx * i] <= 1)
					{
						pinky_y = pinky_y + plusy * i;
						pinky_x = pinky_x + plusx * i;
						break;
					}
				}
			}
			najblizszy_wierzcholek_pacmana = znajdz_najblizszy_wierzcholek(pinky_y, pinky_x);
			//jeœli pinky jest na docelowym wierzcho³ku, wykonuje ruch w kierunku pacmana
			if (najblizszy_wierzcholek_pacmana == pinky.obecny_wierzcholek)
			{
				najblizszy_wierzcholek_pacmana = znajdz_najblizszy_wierzcholek(y_pacmana, x_pacmana);
			}
			pinky.dijkstra(pinky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana, &lista);
			pinky.stworz_droge(pinky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana);
			pinky.ustal_kierunek(y_pacmana, x_pacmana, 2);
		}
		pinky.wykonaj_ruch();
	}
	///////////////////////////////////////////////
	//Inky
	if (inky_zyje)
	{
		inky.duszek_przekroczyl_tunel();
		if (inky.czy_duszek_jest_na_wierzcholku(tablica_ID))
		{
			int y_blinky, x_blinky;
			int y_inky, x_inky;
			y_blinky = int(blinky.grafika.getPosition().y / 19);
			x_blinky = int(blinky.grafika.getPosition().x / 19);
			//na podstawie wzoru na po³owê odcinka
			x_inky = 2 * x_pacmana - x_blinky;
			y_inky = 2 * y_pacmana - y_blinky;
			//wyjœcie_poza_tablicê
			if (x_inky < 1)
			{
				x_inky = 1;
			}
			else
				if (x_inky > 26)
				{
					x_inky = 26;
				}
			if (y_inky < 1)
			{
				y_inky = 1;
			}
			else
				if (y_inky > 29)
				{
					y_inky = 29;
				}
			//cout << y_inky << " " << x_inky << endl;
			double minimalna_odleglosc = 1000;
			double obecna_odleglosc;
			//sprawdzamy odleg³oœæ ka¿dego wierzcho³ka od celu inkyego
			for (list<list<wierzcholek>>::iterator it = lista.begin(); it != lista.end(); it++)
			{
				//odleg³oœæ wierzcho³ka it od celu inkyego
				obecna_odleglosc = sqrt(pow((it->begin()->y - y_inky), 2) + pow((it->begin()->x - x_inky), 2));
				if (obecna_odleglosc < minimalna_odleglosc)
				{
					minimalna_odleglosc = obecna_odleglosc;
					najblizszy_wierzcholek_pacmana = it->begin()->ID;
				}
			}
			inky.dijkstra(inky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana, &lista);
			inky.stworz_droge(inky.obecny_wierzcholek, najblizszy_wierzcholek_pacmana);
			inky.ustal_kierunek(y_pacmana, x_pacmana, 3);
		}
		inky.wykonaj_ruch();
	}
	////////////////////////////
}


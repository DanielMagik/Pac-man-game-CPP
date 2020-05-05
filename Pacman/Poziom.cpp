#include "Poziom.h"
#include <sstream>
#include <deque>

//wzorzec do kolizji
template <class K1, class K2> bool kolizja(K1  &a, K2 &b)
{
	//sprawdzenie dla ka�dej strony, czy 
	//czy origin obiektu b znajduje si� wewn�trz tekstury a
	return (b.grafika.getPosition().x > a.grafika.getPosition().x - a.grafika.getOrigin().x
		&&  b.grafika.getPosition().x < a.grafika.getPosition().x + a.grafika.getOrigin().x
		&&  b.grafika.getPosition().y > a.grafika.getPosition().y - a.grafika.getOrigin().y
		&&  b.grafika.getPosition().y < a.grafika.getPosition().y + a.grafika.getOrigin().y);
}

Poziom::Poziom()
{
	//ustawienie napis�w bocznych
	czcionka.loadFromFile("arial.ttf");
	napis_punkty.setFont(czcionka);
	napis_punkty.setString("Punkty: ");
	napis_punkty.setCharacterSize(40);
	napis_punkty.setPosition(540, 0);
	liczba_punktow.setFont(czcionka);
	liczba_punktow.setCharacterSize(40);
	liczba_punktow.setPosition(540, 40);
	napis_zycia.setFont(czcionka);
	napis_zycia.setString(L"�ycia:");
	napis_zycia.setCharacterSize(40);
	napis_zycia.setPosition(540, 80);
	liczba_zyc.setFont(czcionka);
	liczba_zyc.setCharacterSize(40);
	liczba_zyc.setPosition(540, 120);

	//pobranie informancji o statusie gry
	plik.open("PLIKI/ZAPIS.txt", ios::in);
	plik >> obecny_level;
	plik >> punkty;
	punkty_na_poczatku_levelu = punkty;
	plik >> zycia;
	zycia_na_poczatku = zycia;
	plik.close();
	//przekazanie informacji do wy�wietlania punkt�w i �y�
	ostringstream nowe_punkty;
	nowe_punkty << punkty;
	string nowy_napis = nowe_punkty.str();
	liczba_punktow.setString(nowy_napis);
	ostringstream nowe_zycia;
	nowe_zycia << zycia;
	nowy_napis = nowe_zycia.str();
	liczba_zyc.setString(nowy_napis);
	tekstura_kropki.loadFromFile("GRAFIKA/kropka.png");
	tekstura_duzej_kropki.loadFromFile("GRAFIKA/duza_kropka.png");
	tekstura_smiertelnego_duszka.loadFromFile("GRAFIKA/smiertelny_duszek.png");
}
//w�a�ciwa funkcja obs�uguj�ca gr�, p�tla g��wna gry
void Poziom::uruchom(RenderWindow * okno)
{
	int licznik_petli = 0;//zmienna s�u�y do zmiany pr�dko�ci obiekt�w
	if (zresetuj)
	{
		restart_poziomu();
		zresetuj = false;
	}
	//ustawienie jednej grafiki bloczk�w
	bloczki.begin()->tekstura.loadFromFile("GRAFIKA/bloczek.png");
	//kazdy bloczek korzysta z wcze�niej za�adowanej tekstury
	for (deque<Bloczek>::iterator it = bloczki.begin(); it != bloczki.end(); it++)
	{
		it->grafika.setTexture(bloczki.begin()->tekstura);
	}
	//�adowanie grafik kropek. Korzystamy z zapami�tanej tekstury kropki
	for (deque<Kropka>::iterator it = kropki.begin(); it != kropki.end(); it++)
	{
		it->grafika.setTexture(tekstura_kropki);
	}

	for (deque<Kropka>::iterator it = duze_kropki.begin(); it != duze_kropki.end(); it++)
	{
		it->grafika.setTexture(tekstura_duzej_kropki);
	}
	while (true)
	{
		okno->pollEvent(event);
		wcisnieto_escape(okno);
		pacman_zjadl_kropke();
		if (licznik_petli %16<15)
		{
			if (pacman_niesmiertelny == false)
			{
				graf.logika_duszkow(pacman);
			}
		}
		zabito_pacmana(okno);
		if (licznik_kropek ==0)//koniec poziomu
		{
			if (obecny_level==3)//ostatni poziom, wygrana
			{
				wygrana(okno);
			}
			else//koniec poziomu
			{
				punkty_na_poczatku_levelu = punkty;
				Po_poziomie p = koniec_levela;
				throw p;
			}
		}
		//wyznaczanie gdzie mo�na skr�ci� musi odby� si� przed ruchem
		gdzie_mozna_skrecac();
		kierunek_pacmana();
		okno->clear();
		//spowolnienie ruch�w, tylko co pewn� klatk�
		if (licznik_petli == 100)
		{
			licznik_petli = 0;
		}
		else
		{
			ruch();
			licznik_petli++;
		}
		przekroczenie_tunelu();
		//sprawdzanie czasu wyswietlania wisienki
		if (wyswietl_wisienke)
		{
			obecny_czas_wisienki = clock();
			czas = double((obecny_czas_wisienki - start_wisienki) / CLOCKS_PER_SEC);
			if (czas > 5)
			{
				wyswietl_wisienke = false;
			}
		}
		//sprawdzenie czasu nie�miertelno�ci
		if (pacman_niesmiertelny)
		{
			obecny_czas_niesmiertelnosci = clock();
			czas_niesmiertelnosci = double((obecny_czas_niesmiertelnosci - start_niesmiertelnosci) / CLOCKS_PER_SEC);
			if (czas_niesmiertelnosci > 1 && zmieniono_teksture_duszkow==false)
			{
				zmieniono_teksture_duszkow = true;
				graf.blinky.grafika.setTexture(graf.blinky.tekstura);
				graf.pinky.grafika.setTexture(graf.pinky.tekstura);
				graf.inky.grafika.setTexture(graf.inky.tekstura);
				graf.clyde.grafika.setTexture(graf.clyde.tekstura);
			}
			if (czas_niesmiertelnosci > 2)
			{
				pacman_niesmiertelny = false;
			}
			//Sprawdzenie kolizji z duszkami
			bool zjedzono_duszka = false;
			if (kolizja(pacman, graf.blinky))
			{
				graf.blinky.teleport_do_domku(graf.y_domku, graf.x_domku);
				zjedzono_duszka = true;
			}
			if (kolizja(pacman, graf.pinky))
			{
				graf.pinky.teleport_do_domku(graf.y_domku, graf.x_domku);
				zjedzono_duszka = true;
			}
			if (kolizja(pacman, graf.inky))
			{
				graf.inky.teleport_do_domku(graf.y_domku, graf.x_domku);
				zjedzono_duszka = true;
			}
			if (kolizja(pacman, graf.clyde))
			{
				graf.clyde.teleport_do_domku(graf.y_domku, graf.x_domku);
				zjedzono_duszka = true;
			}
			if (zjedzono_duszka)
			{
				punkty += 50;
			}
			//stworzenie napisu nowej ilo�ci punkt�w
			ostringstream nowe_punkty;
			nowe_punkty << punkty;
			string nowy_napis = nowe_punkty.str();
			liczba_punktow.setString(nowy_napis);
		}

		//rysowanie grafiki
		wyswietl_bloczki(okno);
		wyswietl_kropki(okno);
		if (wyswietl_wisienke)
		{
			okno->draw(wisienka.grafika);
		}
		okno->draw(pacman.grafika);
		okno->draw(napis_punkty);
		okno->draw(liczba_punktow);
		okno->draw(napis_zycia);
		okno->draw(liczba_zyc);
		graf.wyswietl(okno);
		okno->display();
	}
}
//sprawdzamy, czy pacman zjad� kropk�, du�� kropk� lub wisienk�
void Poziom::pacman_zjadl_kropke()
{
	int px, py;
	//ustalenie pozycji pacmana (konkretna kratka w tablicy)
	px = int(pacman.grafika.getPosition().x / 19);
	py = int(pacman.grafika.getPosition().y / 19);
	//sprawdzenie kolizji z ka�d� kropk�
	for (deque<Kropka>::iterator it = kropki.begin(); it != kropki.end(); it++)
	{
		//sprawdzenie, czy kropka zajduje si� w tej samej kratce, co pacman
		if (it->px == px && it->py == py)
		{
			if (kolizja(pacman, *it))
			{
				//usuni�cie zjedzonej kropki
				kropki.erase(it);
				licznik_kropek--;
				if (licznik_kropek == int(0.7*poczatkowa_liczba_kropek) ||
					licznik_kropek == int(0.4*poczatkowa_liczba_kropek))
				{
					wyswietl_wisienke = true;
					start_wisienki = clock();
				}
				punkty += 10;
				//stworzenie napisu nowej ilo�ci punkt�w
				ostringstream nowe_punkty;
				nowe_punkty << punkty;
				string nowy_napis = nowe_punkty.str();
				liczba_punktow.setString(nowy_napis);
				
				//////////////////////////////////////

				//spowolnienie pacmana po zebraniu kropki
			if (pacman.predkosc.x == -1 )
			{
				pacman.grafika.move(2.375, 0);
			}
			else
			if (pacman.predkosc.x == 1 )
			{
				pacman.grafika.move(-2.375, 0);
			}
			else
			if (pacman.predkosc.y == -1)
			{
				pacman.grafika.move(0, 2.375);
			}
			else
			if (pacman.predkosc.y == 1 )
			{
				pacman.grafika.move(0, -2.375);
			}
				//////////////////////////////////////
				break;//wyj�cie z p�tli po zjedzeniu kropki
			}
		}
	}
	//Du�e kropki
	for (deque<Kropka>::iterator it = duze_kropki.begin(); it != duze_kropki.end(); it++)
	{
			if (kolizja(pacman, *it))
			{
				//usuni�cie zjedzonej kropki
				duze_kropki.erase(it);
				licznik_kropek--;
				pacman_niesmiertelny = true;
				start_niesmiertelnosci = clock();
				zmieniono_teksture_duszkow = false;
				//zmiana tekstur duszk�w
				graf.blinky.grafika.setTexture(tekstura_smiertelnego_duszka);
				graf.pinky.grafika.setTexture(tekstura_smiertelnego_duszka);
				graf.inky.grafika.setTexture(tekstura_smiertelnego_duszka);
				graf.clyde.grafika.setTexture(tekstura_smiertelnego_duszka);
				if (licznik_kropek == int(0.7*poczatkowa_liczba_kropek) ||
					licznik_kropek == int(0.4*poczatkowa_liczba_kropek))
				{
					wyswietl_wisienke = true;
					start_wisienki = clock();
				}
				break;//wyj�cie z p�tli po zjedzeniu du�ej kropki
			}
	}
	///////////////////////////
	//sprawdzamy, czy pacman zjad� wisienk�
	if (wyswietl_wisienke && kolizja(pacman, wisienka))
	{
		wyswietl_wisienke = false;
		punkty += 300;
		//stworzenie napisu nowej ilo�ci punkt�w
		ostringstream nowe_punkty;
		nowe_punkty << punkty;
		string nowy_napis = nowe_punkty.str();
		liczba_punktow.setString(nowy_napis);
	}
}

//czy wci�ni�to escape(odpowiednie procedury) lub czy wci�ni�to x
void Poziom::wcisnieto_escape(RenderWindow * okno)
{
	//wci�ni�to x
	if (event.type == Event::Closed)
	{
		Po_poziomie p = wyjscie_z_gry;
		throw p;
	}
	//wci�ni�to escape
	if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
	{
		while (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {}
		//puszczono escape
		Po_poziomie p = nic;//co robi� po zamkni�ciu okna
		Texture tekstura_ramka;
		Sprite ramka;
		Texture escape_tekstura;
		Sprite escape;
		//ustawienie grafiki dla okna i ramki
		tekstura_ramka.loadFromFile("GRAFIKA/escape_ramka.png");
		escape_tekstura.loadFromFile("GRAFIKA/escape.png");
		ramka.setTexture(tekstura_ramka);
		escape.setTexture(escape_tekstura);
		escape.setPosition(300, 200);
		ramka.setPosition(300, 220);
		try
		{
			while (true)
			{
				//wy�wietlanie okna i ramki
				okno->pollEvent(event);
				okno->draw(escape);
				okno->draw(ramka);
				okno->display();
				//naci�ni�to x
				if (event.type == Event::Closed)
				{
					p = wyjscie_z_gry;
					throw p;
				}
				//ponownie naci�ni�to esc
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
				{
					while (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {}
					//wracamy do gry
					p = nic;
					throw p;
				}
				//naci�ni�to Up, zmiana wyboru
				if (Keyboard::isKeyPressed(Keyboard::Key::Up) && p != nic)
				{
					while (Keyboard::isKeyPressed(Keyboard::Key::Up)) {}
					ramka.move(0, -51);
					switch (p)
					{
					case reset_levela:
						p = nic;
						break;
					case wyjscie_z_gry:
						p = reset_levela;
						break;
					}
				}
				//naci�ni�to Down, zmiana wyboru
				if (Keyboard::isKeyPressed(Keyboard::Key::Down) && p != wyjscie_z_gry)
				{
					while (Keyboard::isKeyPressed(Keyboard::Key::Down)) {}
					ramka.move(0, 51);
					switch (p)
					{
					case reset_levela:
						p = wyjscie_z_gry;
						break;
					case nic:
						p = reset_levela;
						break;
					}
				}
				//naci�ni�to enter
				if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
				{
					//wybrano reset levela
					if (p == reset_levela)
					{
						zresetuj = true;
					}
					throw p;
				}
			}
		}
		//po przerwaniu p�tli
		catch (Po_poziomie p)
		{
			if(p!=nic)
			throw p;
		}
	}
}
//pojedynczy ruch pacmana w jednej klatce
void Poziom::ruch()
{
	if (pacman.predkosc.x == -1 && pacman.mozna_w_lewo)
	{
		pacman.grafika.move(-2.375,0);
	}
	else
	if (pacman.predkosc.x == 1 && pacman.mozna_w_prawo)
	{
		pacman.grafika.move(2.375,0);
	}
	else
	if (pacman.predkosc.y == -1 && pacman.mozna_w_gore)
	{
		pacman.grafika.move(0,-2.375);
	}
	else
	if (pacman.predkosc.y == 1 && pacman.mozna_w_dol)
	{
		pacman.grafika.move(0, 2.375);
	}
}
//wy�wietlanie ca�ej listy bloczk�w
void Poziom::wyswietl_bloczki(RenderWindow * okno)
{
	for (deque<Bloczek>::iterator it = bloczki.begin(); it != bloczki.end(); it++)
	{
		okno->draw(it->grafika);
	}
}
//wy�wietlanie ca�ej listy kropek i du�ych kropek
void Poziom::wyswietl_kropki(RenderWindow * okno)
{
	for (deque<Kropka>::iterator it = kropki.begin(); it != kropki.end(); it++)
	{
		okno->draw(it->grafika);
	}
	for (deque<Kropka>::iterator it = duze_kropki.begin(); it != duze_kropki.end(); it++)
	{
		okno->draw(it->grafika);
	}
}
//ustalanie kierunku pacmana
void Poziom::kierunek_pacmana()
{
	//je�li wci�ni�ty jest dany klawisz i mo�na skr�ci�, to pacman zmienia kierunek
	if ((Keyboard::isKeyPressed(Keyboard::Key::Up) 
		|| Keyboard::isKeyPressed(Keyboard::Key::W)) && pacman.mozna_w_gore)
	{
		pacman.predkosc.y = -1;
		pacman.predkosc.x = 0;
		pacman.grafika.setTexture(pacman.tekstura_gora);
	}
	if ((Keyboard::isKeyPressed(Keyboard::Key::Down)
		|| Keyboard::isKeyPressed(Keyboard::Key::S)) && pacman.mozna_w_dol)
	{
		pacman.predkosc.y = 1;
		pacman.predkosc.x = 0;
		pacman.grafika.setTexture(pacman.tekstura_dol);
	}
	if ((Keyboard::isKeyPressed(Keyboard::Key::Right)
		|| Keyboard::isKeyPressed(Keyboard::Key::D)) && pacman.mozna_w_prawo)
	{
		pacman.predkosc.y = 0;
		pacman.predkosc.x = 1;
		pacman.grafika.setTexture(pacman.tekstura_prawo);
	}
	if ((Keyboard::isKeyPressed(Keyboard::Key::Left)
		|| Keyboard::isKeyPressed(Keyboard::Key::A)) && pacman.mozna_w_lewo)
	{
		pacman.predkosc.y = 0;
		pacman.predkosc.x = -1;
		pacman.grafika.setTexture(pacman.tekstura_lewo);
	}
}
//ustalanie, gdzie pacman mo�e skr�ci�
void Poziom::gdzie_mozna_skrecac()
{
	//czy pacman nie znajduje si� na granicy mapy, by nie wyj�� poza zakres tablicy 
	if (pacman.grafika.getPosition().x >= 19 && 
		pacman.grafika.getPosition().x <=513 
		&& pacman.grafika.getPosition().y >= 19
		&& pacman.grafika.getPosition().y <= 570)
	{
		int px, py;
		//ustalenie pola tablicy w kt�rym jest �rodek pacmana
		px = int(pacman.grafika.getPosition().x);
		py = int(pacman.grafika.getPosition().y);
		int tablicax = int(px / 19), tablicay = int(py / 19);
		//�rodek pacmana jest w �rodku pola
		if (px % 19 == 10 && py % 19 == 10)
		{
			pacman.px = tablicax;
			pacman.py = tablicay;
			graf.pobierz_wspolrzedne_pacmana(pacman);
			//reset mo�liwo�ci
			pacman.mozna_w_dol = false;
			pacman.mozna_w_gore = false;
			pacman.mozna_w_prawo = false;
			pacman.mozna_w_lewo = false;
			//sprawdzanie, czy dla odpowiedniego pola tablicy jest mo�liwo��, by
			//pacman tam skr�ci�. Sprawdzamy 4 s�siednie pola pacmana
			if (tablica[tablicay + 1][tablicax] >= 0 && tablica[tablicay + 1][tablicax] <=5)
			{
				pacman.mozna_w_dol = true;
			}
			if (tablica[tablicay - 1][tablicax] >=0 && tablica[tablicay - 1][tablicax] <=5)
			{
				pacman.mozna_w_gore = true;
			}
			if (tablica[tablicay][tablicax + 1] >= 0 && tablica[tablicay][tablicax + 1] <= 5)
			{
				pacman.mozna_w_prawo = true;
			}
			if (tablica[tablicay][tablicax - 1] >= 0 && tablica[tablicay][tablicax - 1] <=5)
			{
				pacman.mozna_w_lewo = true;
			}
		}
		//pacman nie znajduje si� na �rodku
		else
		{
			//reset mo�liwo�ci
			pacman.mozna_w_dol = false;
			pacman.mozna_w_gore = false;
			pacman.mozna_w_prawo = false;
			pacman.mozna_w_lewo = false;
			//je�li pacman porusza si� w lewo, to mo�e te� w prawo, podobnie,
			//je�li porusza si� w g�r�, mo�e te� w d�
			if (pacman.predkosc.x == -1 || pacman.predkosc.x == 1)
			{
				pacman.mozna_w_prawo = true;
				pacman.mozna_w_lewo = true;
			}
			if (pacman.predkosc.y == -1 || pacman.predkosc.y == 1)
			{
				pacman.mozna_w_dol = true;
				pacman.mozna_w_gore = true;
			}
			
		}
	}
}
//sprawdzanie, czy pacman wyszed� poza map� (obs�uga tuneli)
void Poziom::przekroczenie_tunelu()
{
	//pacman wyszed� za bardzo w lewo
	if (pacman.grafika.getPosition().x < -18)
	{
		pacman.grafika.setPosition(542, pacman.grafika.getPosition().y);
	}
	else
		//pacman wyszed� za bardzo w prawo
		if (pacman.grafika.getPosition().x > 542)
		{
			pacman.grafika.setPosition(-18, pacman.grafika.getPosition().y);
		}

	//pacman wyszed� za bardzo w g�r�
	if (pacman.grafika.getPosition().y < -18)
	{
		pacman.grafika.setPosition(pacman.grafika.getPosition().x, 599);
	}
	else
		//pacman wyszed� za bardzo w d�
		if (pacman.grafika.getPosition().y > 599)
		{
			pacman.grafika.setPosition(pacman.grafika.getPosition().x, -18);
		}
}
//restart poziomu, ponowne stworzenie list, grafu, 
void Poziom:: restart_poziomu()
{
	//reset licznika punkt�w i liczby �y�
	punkty = punkty_na_poczatku_levelu;
	zycia = zycia_na_poczatku;
	ostringstream nowe_punkty;
	nowe_punkty << punkty;
	string nowy_napis = nowe_punkty.str();
	liczba_punktow.setString(nowy_napis);
	ostringstream nowe_zycia;
	nowe_zycia << zycia;
	nowy_napis = nowe_zycia.str();
	liczba_zyc.setString(nowy_napis);
	//reset kierunku i tekstury pacmana
	pacman.predkosc.y = 0;
	pacman.predkosc.x = 0;
	//odwo�anie ewentualnej du�ej kropki
	if (pacman_niesmiertelny == true)
	{
		pacman_niesmiertelny = false;
		wyswietl_wisienke = false;
		graf.blinky.grafika.setTexture(graf.blinky.tekstura);
		graf.pinky.grafika.setTexture(graf.pinky.tekstura);
		graf.inky.grafika.setTexture(graf.inky.tekstura);
		graf.clyde.grafika.setTexture(graf.clyde.tekstura);
		
	}

	switch (kierunek)
	{
	case 1:
		pacman.predkosc.x = -1;
		pacman.grafika.setTexture(pacman.tekstura_lewo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 2:
		pacman.predkosc.y = 1;
		pacman.grafika.setTexture(pacman.tekstura_dol);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	case 3:
		pacman.predkosc.x = 1;
		pacman.grafika.setTexture(pacman.tekstura_prawo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 4:
		pacman.predkosc.y = -1;
		pacman.grafika.setTexture(pacman.tekstura_gora);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	}

	//ustawienie pozycji pacmana
	pacman.grafika.setPosition(poczatkowe_x * 19 + pacman.grafika.getOrigin().x, poczatkowe_y * 19 + pacman.grafika.getOrigin().y);
	pacman.px = poczatkowe_x;
	pacman.py = poczatkowe_y;
	kropki.clear();
	duze_kropki.clear();
	licznik_kropek = 0;
	//tworzenie nowej listy kropek
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			if (tablica[i][j] == 0)
			{
				//dodawanie bloczka do wektora bloczk�w
				unique_ptr < Kropka > nowa(new Kropka);
				nowa->py = i;
				nowa->px = j;
				nowa->grafika.setPosition(19 * j + 10, 19 * i + 10);
				kropki.push_back(*nowa);
				licznik_kropek++;
			}
			else
			if (tablica[i][j] == 1)
			{
				unique_ptr < Kropka > nowa(new Kropka);
				nowa->py = i;
				nowa->px = j;
				nowa->grafika.setPosition(19 * j + 10, 19 * i + 10);
				duze_kropki.push_back(*nowa);
				licznik_kropek++;
			}
		}
	}
	poczatkowa_liczba_kropek = licznik_kropek;
}
//wczytanie planszy z pliku
void Poziom::wczytaj()
{

	if (pacman_niesmiertelny == true)
	{
		pacman_niesmiertelny = false;
		wyswietl_wisienke = false;
		graf.blinky.grafika.setTexture(graf.blinky.tekstura);
		graf.pinky.grafika.setTexture(graf.pinky.tekstura);
		graf.inky.grafika.setTexture(graf.inky.tekstura);
		graf.clyde.grafika.setTexture(graf.clyde.tekstura);

	}
	bloczki.clear();
	kropki.clear();
	duze_kropki.clear();
	licznik_kropek = 0;
	//pobranie informancji o statusie gry
	plik.open("PLIKI/ZAPIS.txt", ios::in);
	plik >> obecny_level;
	plik >> punkty;
	plik >> zycia;
	zycia_na_poczatku = zycia;
	plik.close();
	//utworzenie nazwy pliku opisuj�cego konkretny level
	string nazwa_pliku;
	nazwa_pliku = "PLIKI/MAPA_";
	ostringstream numer_levelu;
	numer_levelu << obecny_level;
	nazwa_pliku += numer_levelu.str();
	nazwa_pliku += ".txt";
	//otwarcie pliku dla pewnego levela
	plik.open(nazwa_pliku, ios::in);
	//int kierunek;
	plik >> kierunek;
	//ustawienie pr�dko�ci pocz�tkowej pacmana
	switch (kierunek)
	{
	case 1:
		pacman.predkosc.x = -1;
		pacman.grafika.setTexture(pacman.tekstura_lewo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 2:
		pacman.predkosc.y = 1;
		pacman.grafika.setTexture(pacman.tekstura_dol);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	case 3:
		pacman.predkosc.x = 1;
		pacman.grafika.setTexture(pacman.tekstura_prawo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 4:
		pacman.predkosc.y = -1;
		pacman.grafika.setTexture(pacman.tekstura_gora);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	}

	plik >> poczatkowe_x;
	plik >> poczatkowe_y;
	//ustawienie pozycji pacmana
	pacman.grafika.setPosition(poczatkowe_x * 19 + pacman.grafika.getOrigin().x, poczatkowe_y * 19 + pacman.grafika.getOrigin().y);
	pacman.px = poczatkowe_x;
	pacman.py = poczatkowe_y;

	//kopiowanie tablicy z pliku, dynamiczna alokacja tablicy
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			plik >> tablica[i][j];
			//je�li w kom�rce tablicy jest opisany bloczek
			if (tablica[i][j] == 9)
			{
				//dodawanie bloczka do wektora bloczk�w
				unique_ptr < Bloczek > nowy( new Bloczek);
				//Bloczek nowy;
				nowy->py = i;
				nowy->px = j;
				nowy->grafika.setPosition(19 * j, 19 * i);
				bloczki.push_back(*nowy);
			}
			else
			if (tablica[i][j] == 0)
			{
				//dodawanie bloczka do wektora bloczk�w
				unique_ptr < Kropka > nowa(new Kropka);
				nowa->py = i;
				nowa->px = j;
				nowa->grafika.setPosition(19 * j + 10, 19 * i + 10);
				kropki.push_back(*nowa);
				licznik_kropek++;
			}
			//ustawianie w�a�ciwo�ci owocu
			else
			//du�e kropki
			if (tablica[i][j] == 1)
			{
				unique_ptr < Kropka > nowa(new Kropka);
				nowa->py = i;
				nowa->px = j;
				nowa->grafika.setPosition(19 * j + 10, 19 * i + 10);
				duze_kropki.push_back(*nowa);
				licznik_kropek++;
			}
			else
			if (tablica[i][j] == 4)
			{
				wisienka.tekstura.loadFromFile("GRAFIKA/wisienka.png");
				wisienka.grafika.setTexture(wisienka.tekstura);
				wisienka.grafika.setPosition(19 * j + 10, 19 * i + 10);
			}
			
		}
	}
	poczatkowa_liczba_kropek = licznik_kropek;
	graf.zresetuj();
	graf.skopiuj_tablice(&tablica[0][0]);
	graf.stworz_tablice();
	graf.zaznacz_wierzcholki();
	graf.uzupelnij_liste();
	plik.close();
	//losowe tasowanie kropek, by gra chodzi�a r�wno
	random_shuffle(kropki.begin(), kropki.end());
}
//otwieranie okna w przypadku wygrania gry
void Poziom::wygrana(RenderWindow * okno)
{
	//tworzenie napisu
	Texture tekstura_wygranej;
	Sprite grafika_wygranej;
	tekstura_wygranej.loadFromFile("GRAFIKA/wygrana.png");
	grafika_wygranej.setTexture(tekstura_wygranej);
	liczba_punktow.setPosition(0, 295);
	liczba_punktow.setCharacterSize(60);
	//wy�wietlanie okna
	while (true)
	{
		okno->pollEvent(event);
		if (event.type == Event::Closed)
		{
			Po_poziomie p = wyjscie_z_gry;
			throw p;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			Po_poziomie p = wyjscie_z_gry;
			throw p;
		}
		okno->clear();
		okno->draw(grafika_wygranej);
		okno->draw(liczba_punktow);
		okno->display();
	}
}
//sprawdzenie, czy duszki koliduj� z pacmanem
void Poziom::zabito_pacmana(RenderWindow * okno)
{
	if (pacman_niesmiertelny==false && ( kolizja(graf.blinky, pacman) || 
		kolizja(graf.pinky, pacman) ||
		kolizja(graf.inky, pacman) ||
		kolizja(graf.clyde, pacman)))
	{
		//koniec wyswietlania ewentualnej wisienki
		wyswietl_wisienke = false;
		zycia--;
		if (zycia == 0)
		{
			koniec_gry(okno);
		}
		ostringstream nowe_zycia;
		nowe_zycia << zycia;
		string nowy_napis;
		nowy_napis = nowe_zycia.str();
		liczba_zyc.setString(nowy_napis);
		powrot_na_start(okno);
		graf.blinky.teleport_do_domku(graf.y_domku, graf.x_domku);
		graf.pinky.teleport_do_domku(graf.y_domku, graf.x_domku);
		graf.inky.teleport_do_domku(graf.y_domku, graf.x_domku);
		graf.clyde.teleport_do_domku(graf.y_domku, graf.x_domku);
	}
	
}
//obs�uga wy�wietlania ko�ca gry
void Poziom :: koniec_gry (RenderWindow * okno)
{
	Texture tekstura_konca;
	Sprite grafika_konca;
	tekstura_konca.loadFromFile("GRAFIKA/koniec_gry.png");
	grafika_konca.setTexture(tekstura_konca);
	liczba_punktow.setPosition(0, 295);
	liczba_punktow.setCharacterSize(60);
	while (true)
	{
		okno->pollEvent(event);
		if (event.type == Event::Closed)
		{
			Po_poziomie p = wyjscie_z_gry;
			throw p;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			Po_poziomie p = wyjscie_z_gry;
			throw p;
		}
		okno->clear();
		okno->draw(grafika_konca);
		okno->draw(liczba_punktow);
		okno->display();
	}
}
//przemieszczenie pacmana na pocz�tek w przypadku �mierci lub resetu
void Poziom::powrot_na_start(RenderWindow * okno)
{
	pacman.predkosc.y = 0;
	pacman.predkosc.x = 0;
	switch (kierunek)
	{
	case 1:
		pacman.predkosc.x = -1;
		pacman.grafika.setTexture(pacman.tekstura_lewo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 2:
		pacman.predkosc.y = 1;
		pacman.grafika.setTexture(pacman.tekstura_dol);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	case 3:
		pacman.predkosc.x = 1;
		pacman.grafika.setTexture(pacman.tekstura_prawo);
		pacman.mozna_w_lewo = true;
		pacman.mozna_w_prawo = true;
		break;
	case 4:
		pacman.predkosc.y = -1;
		pacman.grafika.setTexture(pacman.tekstura_gora);
		pacman.mozna_w_dol = true;
		pacman.mozna_w_gore = true;
		break;
	}

	//ustawienie pozycji pacmana
	pacman.grafika.setPosition(poczatkowe_x * 19 + pacman.grafika.getOrigin().x, poczatkowe_y * 19 + pacman.grafika.getOrigin().y);
	pacman.px = poczatkowe_x;
	pacman.py = poczatkowe_y;
}
//zmiana liczby punkt�w i �y� na t� na pocz�tku poziomu
void Poziom:: zeruj_punkty()
{
	punkty = 0;
	punkty_na_poczatku_levelu = 0;
	zycia = 100;
	zycia_na_poczatku = 10;
	ostringstream nowe_punkty;
	nowe_punkty << punkty;
	string nowy_napis = nowe_punkty.str();
	liczba_punktow.setString(nowy_napis);
	ostringstream nowe_zycia;
	nowe_zycia << zycia;
	nowy_napis = nowe_zycia.str();
	liczba_zyc.setString(nowy_napis);
}

#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<windows.h>
#include<fstream>
#include<deque>
#include"Pacman.h"
#include"Bloczek.h"
#include"Kropka.h"
#include"graf.h"
using namespace sf;

enum Po_poziomie
{
	koniec_levela = 0,
	reset_levela = 1,
	wyjscie_z_gry = 2,		//co robiæ po zakoñczeniu poziomu
	nic = 3
};
class Poziom
{
	friend class Gra;
	short int obecny_level;
	int punkty;
	int punkty_na_poczatku_levelu;
	int zycia, zycia_na_poczatku;
	short int tablica[31][28];			//pocz¹tek dwuwymiarowej tablicy do opisu planszy
	short int poczatkowe_x, poczatkowe_y;
	short int kierunek;
	fstream plik;				    //pobieranie danych z plików
	bool zresetuj = false;		   //czy zresetowaæ poziom
	Event event;
	Pacman pacman;				
	deque<Bloczek>bloczki;		//bloczki wszystkich œcian
	deque<Kropka> kropki;//wszystkie kropki
	deque<Kropka> duze_kropki;
	Kropka wisienka;
	short int licznik_kropek = 0;
	short int poczatkowa_liczba_kropek;
	Font czcionka;
	Text napis_punkty, liczba_punktow;
	Text napis_zycia, liczba_zyc;
	Texture tekstura_kropki, tekstura_duzej_kropki, tekstura_smiertelnego_duszka;
	Graf graf;
	bool wyswietl_wisienke = false;
	clock_t start_wisienki, obecny_czas_wisienki;
	double czas;
	bool pacman_niesmiertelny = false;
	clock_t start_niesmiertelnosci, obecny_czas_niesmiertelnosci;
	double czas_niesmiertelnosci;
	bool zmieniono_teksture_duszkow = false;

public:
	Poziom();
	//w³aœciwa funkcja obs³uguj¹ca grê, pêtla g³ówna gry
	void uruchom(RenderWindow * okno);
	//wyœwietlanie ca³ej listy bloczków
	void wyswietl_bloczki(RenderWindow * okno);
	//wyœwietlanie ca³ej listy kropek i du¿ych kropek
	void wyswietl_kropki(RenderWindow * okno);
	//ustalanie kierunku pacmana
	void kierunek_pacmana();
	//ustalanie, gdzie pacman mo¿e skrêciæ
	void gdzie_mozna_skrecac();
	//pojedynczy ruch pacmana w jednej klatce
	void ruch();
	//wczytanie planszy z pliku
	void wczytaj();
	//sprawdzanie, czy pacman wyszed³ poza mapê (obs³uga tuneli)
	void przekroczenie_tunelu();
	//czy wciœniêto escape(odpowiednie procedury) lub czy wciœniêto x
	void wcisnieto_escape(RenderWindow * okno);
	//sprawdzamy, czy pacman zjad³ kropkê, du¿¹ kropkê lub wisienkê
	void pacman_zjadl_kropke();
	//restart poziomu, ponowne stworzenie list, grafu, 
	void restart_poziomu();
	//otwieranie okna w przypadku wygrania gry
	void wygrana(RenderWindow * okno);
	//przemieszczenie pacmana na pocz¹tek w przypadku œmierci lub resetu
	void powrot_na_start(RenderWindow * okno);
	//sprawdzenie, czy duszki koliduj¹ z pacmanem
	void zabito_pacmana(RenderWindow * okno);
	//zmiana liczby punktów i ¿yæ na t¹ na pocz¹tku poziomu
	void zeruj_punkty();
	//obs³uga wyœwietlania koñca gry
	void koniec_gry(RenderWindow * okno);
	~Poziom() = default;
};


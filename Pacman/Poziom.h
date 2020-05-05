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
	wyjscie_z_gry = 2,		//co robi� po zako�czeniu poziomu
	nic = 3
};
class Poziom
{
	friend class Gra;
	short int obecny_level;
	int punkty;
	int punkty_na_poczatku_levelu;
	int zycia, zycia_na_poczatku;
	short int tablica[31][28];			//pocz�tek dwuwymiarowej tablicy do opisu planszy
	short int poczatkowe_x, poczatkowe_y;
	short int kierunek;
	fstream plik;				    //pobieranie danych z plik�w
	bool zresetuj = false;		   //czy zresetowa� poziom
	Event event;
	Pacman pacman;				
	deque<Bloczek>bloczki;		//bloczki wszystkich �cian
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
	//w�a�ciwa funkcja obs�uguj�ca gr�, p�tla g��wna gry
	void uruchom(RenderWindow * okno);
	//wy�wietlanie ca�ej listy bloczk�w
	void wyswietl_bloczki(RenderWindow * okno);
	//wy�wietlanie ca�ej listy kropek i du�ych kropek
	void wyswietl_kropki(RenderWindow * okno);
	//ustalanie kierunku pacmana
	void kierunek_pacmana();
	//ustalanie, gdzie pacman mo�e skr�ci�
	void gdzie_mozna_skrecac();
	//pojedynczy ruch pacmana w jednej klatce
	void ruch();
	//wczytanie planszy z pliku
	void wczytaj();
	//sprawdzanie, czy pacman wyszed� poza map� (obs�uga tuneli)
	void przekroczenie_tunelu();
	//czy wci�ni�to escape(odpowiednie procedury) lub czy wci�ni�to x
	void wcisnieto_escape(RenderWindow * okno);
	//sprawdzamy, czy pacman zjad� kropk�, du�� kropk� lub wisienk�
	void pacman_zjadl_kropke();
	//restart poziomu, ponowne stworzenie list, grafu, 
	void restart_poziomu();
	//otwieranie okna w przypadku wygrania gry
	void wygrana(RenderWindow * okno);
	//przemieszczenie pacmana na pocz�tek w przypadku �mierci lub resetu
	void powrot_na_start(RenderWindow * okno);
	//sprawdzenie, czy duszki koliduj� z pacmanem
	void zabito_pacmana(RenderWindow * okno);
	//zmiana liczby punkt�w i �y� na t� na pocz�tku poziomu
	void zeruj_punkty();
	//obs�uga wy�wietlania ko�ca gry
	void koniec_gry(RenderWindow * okno);
	~Poziom() = default;
};


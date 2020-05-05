#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include"Duszek.h"
#include"Pacman.h"
#include"wierzcholek.h"

class Graf
{
	const int nieskonczonosc = 2000000;
    int tablica[31][28];//tablica z pliku
	int tablica2[31][28];//zmodyfikowana tablica
	int tablica_ID[31][28];//tablica indeks�w wierzcho�k�w
	int najblizszy_wierzcholek_pacmana;
	int x_pacmana = 1, y_pacmana =1;
	int poprzedni_wierzcholek_pacmana;
	int y_domku, x_domku;
	int liczba_wierzcholkow = 0;
	wierzcholek poczatek;//wierzcho�ek w domku
	//lista s�siedztwa
	list<list<wierzcholek>> lista;
	Duszek blinky;
	Duszek pinky;
	Duszek inky;
	Duszek clyde;
	bool blinky_zyje = true, pinky_zyje = true, inky_zyje = true, clyde_zyje = true;
public:
	friend class Poziom;
	Graf();
	//skopiowanie tablicy z wy�szej klasy Poziom
	void skopiuj_tablice(short int * tablica);
	//tworzenie tablicy na podstawie tablicy2 do �atwiejszej analizy
	void stworz_tablice();
	//zaznaczanie wierzcho�k�w w tablicy ID
	void zaznacz_wierzcholki();
	//uzupe�nianie listy s�siedztwa (tworzenie grafu)
	void uzupelnij_liste();
	//reset tablicy ID i grafu
	void zresetuj();
	//wy�wietlanie grafik duszk�w
	void wyswietl(RenderWindow * okno);
	//znajdowanie najbli�szego wierzcho�ka dla (y,x)
	int znajdz_najblizszy_wierzcholek(int y, int x);
	void pobierz_wspolrzedne_pacmana(Pacman pacman);
	//wyznaczanie drogi i ruch duszk�w
	void logika_duszkow(Pacman pacman);
	~Graf() = default;

};


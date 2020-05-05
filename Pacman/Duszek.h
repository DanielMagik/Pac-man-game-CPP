#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<memory>
#include<list>
#include"wierzcholek.h"

using namespace std;
using namespace sf;

class Duszek
{
	friend class Graf;
	friend class Poziom;
	const int nieskonczonosc = 2000000;
	int px, py;//pozycja_duszka
	int obecny_wierzcholek;//obecny wierzcho³ek
	int liczba_wierzcholkow = 0;//liczba wierzcho³ków grafu
	int y_pacmana, x_pacmana;//wspó³rzêdne pacmana
	Vector2f predkosc{ 0,0 };
	Texture tekstura;
	//tablice wierzcho³ków, dystansów, poprzedników i informacji o przetworzeniu oraz
	//lista wierzcho³ków, wykorzystywane w algorytmie Dijkstry
	shared_ptr < wierzcholek *[] > tab_wierzcholkow;
	shared_ptr < int[] > tab_dystans;
	shared_ptr < wierzcholek*[] > tab_poprzednik;//tablice do algorytmu
	shared_ptr < bool[] > tab_przetworzono;
	list<wierzcholek> droga;
	//iloœæ wierzcho³ków w drodze
	int dlugosc_drogi = 0;
public:
	Sprite grafika;
	Duszek();
	//znalezienie wierzcho³ka jeszcze nieprzetworzonego z najmniejsz¹ wartoœci¹ w tablicy
    //dystans. Wykorzystywane w algorytmie Dijkstry
	int znajdz_najmniejszy_nieobsluzony(int meta);
	//pobranie liczby wierzcho³ków z wy¿szej klasy graf
	void pobierz_liczbe_wierzcholkow(int liczba);
	//sprawdzenie, czy grafika duszka wysz³a poza planszê i ewnetualna
	//korekta wspó³rzêdnych
	void duszek_przekroczyl_tunel();
	//sprawdzenie, czy duszek jest na wierzcho³ku grafu, co za tym idzie, czy wyliczaæ drogê
	bool czy_duszek_jest_na_wierzcholku(int tablica_ID[31][28]);
	//inicjalizowanie tablic i listy do algorytmu Dijkstry
	void uzupelnij_duszka(list<list<wierzcholek>> *lista);
	//wyznaczanie drogi od wierzcho³ka start do meta
	void dijkstra(int start, int meta, list <list <wierzcholek>> *lista);
	//tworzenie drogi na podstawie wczeœniej wykonanego algorytmu Dijkstry
	void stworz_droge(int start, int meta);
	//ustalenie prêdkoœci na podstawie drogi
	void ustal_kierunek(int y_pacmana, int x_pacmana, int ID_ducha);
	//wykonanie pojedyñczego ruchu
	void wykonaj_ruch();
	//przemieszczenie grafiki duszka do domku
	void teleport_do_domku(int poczatkowe_y, int poczatkowe_x);
	~Duszek() = default;
};



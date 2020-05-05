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
	int obecny_wierzcholek;//obecny wierzcho�ek
	int liczba_wierzcholkow = 0;//liczba wierzcho�k�w grafu
	int y_pacmana, x_pacmana;//wsp�rz�dne pacmana
	Vector2f predkosc{ 0,0 };
	Texture tekstura;
	//tablice wierzcho�k�w, dystans�w, poprzednik�w i informacji o przetworzeniu oraz
	//lista wierzcho�k�w, wykorzystywane w algorytmie Dijkstry
	shared_ptr < wierzcholek *[] > tab_wierzcholkow;
	shared_ptr < int[] > tab_dystans;
	shared_ptr < wierzcholek*[] > tab_poprzednik;//tablice do algorytmu
	shared_ptr < bool[] > tab_przetworzono;
	list<wierzcholek> droga;
	//ilo�� wierzcho�k�w w drodze
	int dlugosc_drogi = 0;
public:
	Sprite grafika;
	Duszek();
	//znalezienie wierzcho�ka jeszcze nieprzetworzonego z najmniejsz� warto�ci� w tablicy
    //dystans. Wykorzystywane w algorytmie Dijkstry
	int znajdz_najmniejszy_nieobsluzony(int meta);
	//pobranie liczby wierzcho�k�w z wy�szej klasy graf
	void pobierz_liczbe_wierzcholkow(int liczba);
	//sprawdzenie, czy grafika duszka wysz�a poza plansz� i ewnetualna
	//korekta wsp�rz�dnych
	void duszek_przekroczyl_tunel();
	//sprawdzenie, czy duszek jest na wierzcho�ku grafu, co za tym idzie, czy wylicza� drog�
	bool czy_duszek_jest_na_wierzcholku(int tablica_ID[31][28]);
	//inicjalizowanie tablic i listy do algorytmu Dijkstry
	void uzupelnij_duszka(list<list<wierzcholek>> *lista);
	//wyznaczanie drogi od wierzcho�ka start do meta
	void dijkstra(int start, int meta, list <list <wierzcholek>> *lista);
	//tworzenie drogi na podstawie wcze�niej wykonanego algorytmu Dijkstry
	void stworz_droge(int start, int meta);
	//ustalenie pr�dko�ci na podstawie drogi
	void ustal_kierunek(int y_pacmana, int x_pacmana, int ID_ducha);
	//wykonanie pojedy�czego ruchu
	void wykonaj_ruch();
	//przemieszczenie grafiki duszka do domku
	void teleport_do_domku(int poczatkowe_y, int poczatkowe_x);
	~Duszek() = default;
};



#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
using namespace sf;
//s�u�y do obs�ugi okna wyj�cia z menu
class Wyjscie
{
	bool chce_wyjsc = false;	//wychodzi� czy nie
	Texture okienko;
	Sprite okienko_obraz;
	Event event;
public:
	Wyjscie();
	//w�a�ciwa funkcja obs�uguj�ca wyj�cie
	void wychodzenie(RenderWindow * okno);
	~Wyjscie() = default;
};



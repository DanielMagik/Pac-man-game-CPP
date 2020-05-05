#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
using namespace sf;
//s³u¿y do obs³ugi okna wyjœcia z menu
class Wyjscie
{
	bool chce_wyjsc = false;	//wychodziæ czy nie
	Texture okienko;
	Sprite okienko_obraz;
	Event event;
public:
	Wyjscie();
	//w³aœciwa funkcja obs³uguj¹ca wyjœcie
	void wychodzenie(RenderWindow * okno);
	~Wyjscie() = default;
};



#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
using namespace sf;
//s³u¿y do opisu pojedynczej kropki, du¿ej kropki i nawet wisienki
class Kropka
{
	friend class Poziom;
	short int px, py;	//po³o¿enie kropki w tablicy
	Texture tekstura;
public:
	Sprite grafika;
	Kropka();
	~Kropka() = default;
};


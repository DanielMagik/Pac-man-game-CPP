#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include"wyjscie.h"
using namespace sf;
enum Co_dalej		//co robiæ bo naciœniêciu entera
{
	gra = 0,
	kontynuuj_gre = 1,
	wyjscie = 2
};
enum Wybor
{
	nowa_gra = 0,
	kontynuuj = 1,	//co jest teraz wybrane
	wyjdz = 2
};
class Menu
{
	Texture tekstura_tlo;
	Texture tekstura_ramka;		
	Sprite tlo;
	Sprite ramka;
	Event event;
public:
	Menu();
	//w³aœciwa obs³uga menu
	void main_menu(RenderWindow * window);
	~Menu() = default;
};


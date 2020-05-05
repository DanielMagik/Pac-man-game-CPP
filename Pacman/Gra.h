#pragma once
//#include<SFML/Graphics.hpp>
//#include<SFML/Window.hpp>

#include"Menu.h";
#include"Poziom.h";

using namespace sf;
//ogs³uga g³ównej struktury gry, zawiera menu oraz poziom
class Gra
{
	bool uruchom_menu = true;	//czy menu czy gra
	HWND okno;					//zamykanie konsoli
	sf::RenderWindow window{ sf::VideoMode(800, 620,32), "Pacman" };//okno
	Menu menu;
	Poziom poziom;
public:
	Gra();
	//w³aœciwa funkcja grania
	void graj();
	//modyfikacja pliku zapamiêtuj¹cego zapis gry
	void reset();
	~Gra() = default;
};


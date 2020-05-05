#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<iostream>
using namespace sf;
class Pacman
{
	friend class Poziom;
	friend class Graf;
	bool mozna_w_lewo = true;
	bool mozna_w_prawo = true;	//gdzie aktualnie mo¿na skrêcaæ
	bool mozna_w_gore = true;
	bool mozna_w_dol = true;
	Vector2f predkosc{ 0,0 };
	Texture tekstura_prawo;
	Texture tekstura_lewo;	
	Texture tekstura_gora;	//tekstury dla kierunków pacmana
	Texture tekstura_dol;
public:
	int py, px;//po³o¿enie pacmana
	Sprite grafika;
	Pacman();
	//zmiana prêdkoœci na (0,0). Wykorzystywane przy resetach gry
	void zeruj_predkosc();
	~Pacman() = default;
};


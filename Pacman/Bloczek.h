#pragma once
using namespace sf;
class Bloczek
{
	friend class Poziom;
	short int px, py;				//po�o�enie bloczka w tablicy
	Texture tekstura;
	Sprite grafika;
public:
	Bloczek() = default;
	~Bloczek() = default;
};


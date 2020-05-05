#pragma once
//s³u¿y do opisu pojedyñczego wierzcho³ka grafu
struct wierzcholek
{
	short unsigned int x;
	short unsigned int y;
	short unsigned int ID;
	short unsigned int odstep;
	wierzcholek();
	~wierzcholek() = default;
};


#pragma once
//s�u�y do opisu pojedy�czego wierzcho�ka grafu
struct wierzcholek
{
	short unsigned int x;
	short unsigned int y;
	short unsigned int ID;
	short unsigned int odstep;
	wierzcholek();
	~wierzcholek() = default;
};


#pragma once
//służy do opisu pojedyńczego wierzchołka grafu
struct wierzcholek
{
	short unsigned int x;
	short unsigned int y;
	short unsigned int ID;
	short unsigned int odstep;
	wierzcholek();
	~wierzcholek() = default;
};


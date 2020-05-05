#include "Pacman.h"

Pacman::Pacman()
{
	//³adowanie 4 grafik
	tekstura_lewo.loadFromFile("GRAFIKA/pacman_lewo.png");
	tekstura_prawo.loadFromFile("GRAFIKA/pacman_prawo.png");
	tekstura_gora.loadFromFile("GRAFIKA/pacman_gora.png");
	tekstura_dol.loadFromFile("GRAFIKA/pacman_dol.png");
	grafika.setTexture(tekstura_lewo);
	//ustawienie œrodka i pozycji
	grafika.setOrigin(10, 10);
}
//zmiana prêdkoœci na (0,0)
void Pacman::zeruj_predkosc()
{
	predkosc.x = 0;
	predkosc.y = 0;
}


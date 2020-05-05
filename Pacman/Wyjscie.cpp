#include "wyjscie.h"

//³adowanie grafik
Wyjscie::Wyjscie()
{
	okienko.loadFromFile("GRAFIKA/wyjscie_tak.png");	
	okienko.loadFromFile("GRAFIKA/wyjscie_nie.png");
	okienko_obraz.setTexture(okienko);   //przpisanie grafiki
	okienko_obraz.setPosition(265, 210);
}
void Wyjscie::wychodzenie(RenderWindow * okno)
{
	okno->pollEvent(event);
	if (event.type == Event::Closed)//naciœniêto x
	{
		bool b = false;
		throw b;
	}
	//przejœcie w dó³ i zmiana grafiki
	if (Keyboard::isKeyPressed(Keyboard::Key::Down) && chce_wyjsc == false)
	{
		chce_wyjsc = true;
		okienko.loadFromFile("GRAFIKA/wyjscie_tak.png");
		okienko_obraz.setTexture(okienko);
	}
	//przejœcie w górê i zmiana grafiki
	if (Keyboard::isKeyPressed(Keyboard::Key::Up) && chce_wyjsc == true)
	{
		chce_wyjsc = false;
		okienko.loadFromFile("GRAFIKA/wyjscie_nie.png");
		okienko_obraz.setTexture(okienko);
	}
	//naciœniêto enter
	if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
	{
		//wychodzenie z gry
		if (chce_wyjsc == true)
		{
			bool b = false;
			throw b;
		}
		//powrót do menu
		else
		{
			bool b = true;
			throw b;
		}
	}
	//rysowanie wyjœcia
	okno->draw(okienko_obraz);
}


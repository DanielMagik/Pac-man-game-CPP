#include "Menu.h"
#include "wyjscie.h"
#include "Gra.h"
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

using namespace sf;

Menu::Menu()
{
	//wczytanie i ustawienie grafiki tekstur 
	tekstura_ramka.loadFromFile("GRAFIKA/ramka.png");
	ramka.setTexture(tekstura_ramka);
	ramka.setPosition(207, 114);
	tekstura_tlo.loadFromFile("GRAFIKA/menu.png");
	tlo.setTexture(tekstura_tlo);
}
//w�a�ciwa obs�uga menu
void Menu::main_menu(RenderWindow * window)
{
	bool okienko=true;//czy da si� wy�wietli� okno wyj�cia, zmienna zrobiona, by
	//to okno nie miga�o
	Wybor w = nowa_gra;//pocz�tkowy wyb�r
	Co_dalej co_dalej;

		bool niewcisnietydown = true;
		bool niewcisnietyup = true;		//czy up/down jest trzymany
		while (true)
		{
			//naci�ni�to x
			window->pollEvent(event);
			if (event.type == Event::Closed)
			{
				co_dalej = wyjscie;
				throw wyjscie;
			}

			//przemieszczenie w dol
			if (Keyboard::isKeyPressed(Keyboard::Key::Down) && w != wyjdz)				{
				if (niewcisnietydown)
				{
					if (w == nowa_gra)
					{
						w = kontynuuj;
					}
					else               //zmiana potencjalnego wyboru
					{
						w = wyjdz;
					}
					ramka.move(0, 107);
					niewcisnietydown = false;
				}
			}
			else
			{
					niewcisnietydown = true;
			}
			//przemieszczenie w gore
			if (Keyboard::isKeyPressed(Keyboard::Key::Up) && w != nowa_gra)
			{
				if (niewcisnietyup)
				{
					if (w == wyjdz)
					{
						w = kontynuuj;
					}
					else			 //zmiana potencjalnego wyboru
					{
						w = nowa_gra;
					}
					ramka.move(0, -107);
					niewcisnietyup = false;
				}
			}
			else
			{
					niewcisnietyup = true;
			}
			//je�li enter nie jest wci�ni�ty, jest mo�liwo�� 
			//ponownego wyj�cia
			if (!Keyboard::isKeyPressed(Keyboard::Key::Enter))
			{
				okienko = true;
			}
				//wcisnieto enter
			if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && okienko)
			{
				while (Keyboard::isKeyPressed(Keyboard::Key::Enter)){}
						//nic nie robi dop�ki enter jest wcisniet
					
				//wybrano wyjscie	
				if (w == wyjdz)
				{
					Wyjscie wyjdz;
					while (true)
					{
						window->draw(tlo);
						window->draw(ramka);//menu dalej jest rysowane
						try
						{
							wyjdz.wychodzenie(window);//wy�wietlanie okna wychodzenia z gry
						}
						catch (bool b)//przerwanie okna wychodzenia
						{
							if (b == false)
							{
								co_dalej = wyjscie;//wyj�cie z gry
								throw wyjscie;
							}
							else
							{
								okienko = false;//koniec wy�wietlania okienka
								break;
							}
						}
						window->display();
					}
				}
				//wybrano now� gr�
				if (w == nowa_gra)
				{
					co_dalej = gra;
					throw co_dalej;
				}
				//wybrano kontynuacj� gry
				if (w == kontynuuj)
				{
					co_dalej = kontynuuj_gre;
					throw co_dalej;
				}
			}
			window->clear();
			window->draw(tlo);
			window->draw(ramka);	//wy�wietlanie tekstur
			window->display();
		}
}

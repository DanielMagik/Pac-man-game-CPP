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
//w³aœciwa obs³uga menu
void Menu::main_menu(RenderWindow * window)
{
	bool okienko=true;//czy da siê wyœwietliæ okno wyjœcia, zmienna zrobiona, by
	//to okno nie miga³o
	Wybor w = nowa_gra;//pocz¹tkowy wybór
	Co_dalej co_dalej;

		bool niewcisnietydown = true;
		bool niewcisnietyup = true;		//czy up/down jest trzymany
		while (true)
		{
			//naciœniêto x
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
			//jeœli enter nie jest wciœniêty, jest mo¿liwoœæ 
			//ponownego wyjœcia
			if (!Keyboard::isKeyPressed(Keyboard::Key::Enter))
			{
				okienko = true;
			}
				//wcisnieto enter
			if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && okienko)
			{
				while (Keyboard::isKeyPressed(Keyboard::Key::Enter)){}
						//nic nie robi dopóki enter jest wcisniet
					
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
							wyjdz.wychodzenie(window);//wyœwietlanie okna wychodzenia z gry
						}
						catch (bool b)//przerwanie okna wychodzenia
						{
							if (b == false)
							{
								co_dalej = wyjscie;//wyjœcie z gry
								throw wyjscie;
							}
							else
							{
								okienko = false;//koniec wyœwietlania okienka
								break;
							}
						}
						window->display();
					}
				}
				//wybrano now¹ grê
				if (w == nowa_gra)
				{
					co_dalej = gra;
					throw co_dalej;
				}
				//wybrano kontynuacjê gry
				if (w == kontynuuj)
				{
					co_dalej = kontynuuj_gre;
					throw co_dalej;
				}
			}
			window->clear();
			window->draw(tlo);
			window->draw(ramka);	//wyœwietlanie tekstur
			window->display();
		}
}

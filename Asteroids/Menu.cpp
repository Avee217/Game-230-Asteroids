#include "Menu.h"
#include<iostream>

//Change menu items
Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("Fonts/DS-DIGIT.ttf"))
	{
		std::cout << "Font Error" << std::endl;
	}

	menu[0].setFont(font);
	menu[0].setCharacterSize(65);
	menu[0].setString("ASTEROIDS");
	menu[0].setFillColor(sf::Color::White);
	menu[0].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setCharacterSize(55);
	menu[1].setString("Play");
	menu[1].setFillColor(sf::Color::White);
	menu[1].setPosition(sf::Vector2f(width / 6, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setCharacterSize(50);
	menu[2].setFillColor(sf::Color(103, 128, 159, 255));
	menu[2].setString("Quit");
	menu[2].setPosition(sf::Vector2f(width / 6, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	selectedItemIndex = 1;
}


Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 1)
	{
		menu[selectedItemIndex].setFillColor(sf::Color(103, 128, 159, 255));
		menu[selectedItemIndex].setCharacterSize(50);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		menu[selectedItemIndex].setCharacterSize(55);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color(103, 128, 159, 255));
		menu[selectedItemIndex].setCharacterSize(50);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		menu[selectedItemIndex].setCharacterSize(55);
	}
}
#include "Missile.h"



Missile::Missile(float x, float y, float radius)
{
	picture.setFillColor(sf::Color::White);
	setSize(radius);
	setPosition(x, y);
	name = "missile";

}

void Missile::setSize(float radius)
{
	picture.setRadius(radius);
	picture.setOrigin(radius, radius);

}

void Missile::setPosition(float x, float y)
{
	picture.setPosition(sf::Vector2f(x, y));
}
#include "SpaceShip.h"

extern bool resetGame;
extern float timeElapsed;

SpaceShip::SpaceShip(float x, float y, float radius)
{
	picture.setFillColor(sf::Color::White);
	setSize(radius);
	setPosition(x, y);
	name = "spaceShip";

}

void SpaceShip::setSize(float radius)
{
	picture.setRadius(radius);
	picture.setOrigin(radius, radius);
}

void SpaceShip::setPosition(float x, float y)
{
	picture.setPosition(sf::Vector2f(x, y));
}

void SpaceShip::changeSpeed(Animation& spaceShipNotMove, Animation& spaceShipMove)
{
	if (!resetGame)
	{
		if (moveleft && speedX > -500.f)
		{
			speedX -= acceleration * timeElapsed;
			animation = spaceShipMove;
		}

		if (moveright && speedX < 500.f)
		{
			speedX += acceleration * timeElapsed;
			animation = spaceShipMove;
		}

		if (moveup && speedY > -500.f)
		{

			speedY -= acceleration * timeElapsed;
			animation = spaceShipMove;
		}

		if (movedown && speedY < 500.f)
		{
			speedY += acceleration * timeElapsed;
			animation = spaceShipMove;
		}


		if (speedX > 0.5 && !moveleft && !moveright)
		{
			speedX -= damping * timeElapsed;
		}
		else if (speedX < -0.5 && !moveleft && !moveright)
		{
			speedX += damping * timeElapsed;
		}
		else if (!moveleft && !moveright)
		{
			speedX = 0;
		}

		if (speedY > 0.5 && !moveup && !movedown)
		{
			speedY -= damping * timeElapsed;
		}
		else if (speedY < -0.5 && !moveup && !movedown)
		{
			speedY += damping * timeElapsed;
		}
		else if (!moveup && !movedown)
		{
			speedY = 0;
		}

		if (!moveup && !movedown && !moveleft && !moveright)
		{
			animation = spaceShipNotMove;
		}
	}


}
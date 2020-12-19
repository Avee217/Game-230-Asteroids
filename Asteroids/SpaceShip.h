#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"


class SpaceShip : public GameObject
{
public:
	bool isInvincible = false;
	bool moveup, movedown, moveleft, moveright;

	float invincibleTime = 1.0f;
	float speedX = 0.0f;
	float speedY = 0.0f;
	float damping = 350.0f;
	float acceleration = 500.0f;
	SpaceShip(float x, float y, float radius);
	void setSize(float radius);
	void setPosition(float x, float y);
	void changeSpeed(Animation &animStand, Animation &animGo);
};
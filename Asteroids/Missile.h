#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <vector>
#include <math.h>

class Missile : public GameObject
{
public:
	float speed = 1000.f;
	Missile(float x, float y, float radius);
	void setSize(float radius);
	void setPosition(float x, float y);
};


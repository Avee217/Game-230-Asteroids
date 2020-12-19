#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <math.h>
#include <sstream>
#include<iostream>
#include <cstdlib>
#include "Asteroid.h"
#include "Missile.h"
#include "SpaceShip.h"
#include "GameObject.h"
#include "Animation.h"
#include "Menu.h"
#include<chrono>


using namespace sf;

void Initiate();
void Update();
void Render();
void HandleInput();
void loadLevel(int level);

Vector2i getBucket(Vector2f position);
void addToBucket(Vector2i b, Asteroid* asteroidObject);
void removeFromBucket(Vector2i b, Asteroid* asteroidObject);
void colissionDetection(GameObject* gameObject, Vector2i b);


const float PI = 3.14159f;

RenderWindow window;

Clock gameClock;
Font font;


Text lifeText;
Text scoreText;
Text screenText;

std::chrono::steady_clock::time_point clockStart = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point clockEnd = std::chrono::steady_clock::now();
float timeElapsed;

int windowWidth = 1366;
int windowHeight = 768;

float mouseXPosition = 0.0f;
float mouseYPosition = 0.0f;

float nextshot = 0.0f;
float shotDuration = 0.3f;

int life = 3;
int level = 0;
int score = 0;

bool gameRunning = false;
bool resetGame = false;
bool victory = false;
bool spaceShipCrashed = false;
bool vm = false;
bool gom = false;

std::vector<Asteroid*> asteroids;
std::vector<Missile*> missiles;
std::vector<GameObject*> explosions;

SpaceShip spaceShip(windowWidth / 2, windowHeight / 2, 25);

Texture missileTexture;

Texture spaceShipTexture, menuBackgroundTexture, backgroundTexture, explosionCTexture, asteroidBigTexture, t5, asteroidsmallTexture, explosionSpaceShipTexture;
Sprite background, menuBackground;

Menu menu(windowWidth, windowHeight);


Animation sExplosionAsteroids(explosionCTexture, 0, 0, 256, 256, 48, 0.5);
Animation sAsteroidBig(asteroidBigTexture, 0, 0, 64, 64, 16, 0.2);
Animation sAsteroidSmall(asteroidsmallTexture, 0, 0, 64, 64, 16, 0.2);
Animation sSpaceShipNotMove(spaceShipTexture, 40, 0, 40, 40, 1, 0);
Animation sSpaceShipMove(spaceShipTexture, 40, 40, 40, 40, 1, 0);
Animation sExplosionSpaceShip(explosionSpaceShipTexture, 0, 0, 192, 192, 64, 0.5);


const float BUCKET_WIDTH = 200;
const float BUCKET_HEIGHT = 200;
const int COLUMNS = 14;
const int ROWS = 8;
std::vector<Asteroid*> grid[COLUMNS][ROWS];


SoundBuffer shootBuffer;
SoundBuffer explosionBuffer;

SoundBuffer loseBuffer;

Sound shootSound;
Sound explosionSound;

Sound loseSound;
Music backgroundMusic;
Music menuMusic;
Music victoryMusic;
Music gameOverMusic;


int main()
{
	srand(time(0));
	Initiate();
	window.create(VideoMode(windowWidth, windowHeight), "Asteroid");
	window.setFramerateLimit(30);

	menuMusic.play();
	menuMusic.setLoop(true);
	while (window.isOpen())
	{

		clockEnd = std::chrono::steady_clock::now();
		timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(clockEnd - clockStart).count() / 1000000.0f;
		clockStart = clockEnd;
		HandleInput();
		if (gameRunning && !spaceShipCrashed && !resetGame &&!victory)
		{
			Update();

		}
		Render();

	}

	return 0;
}

void Initiate()
{
	font.loadFromFile("Fonts/DS-DIGIT.ttf");
	backgroundTexture.loadFromFile("Images/Background.png");
	menuBackgroundTexture.loadFromFile("Images/menuBackground.png");
	spaceShipTexture.loadFromFile("Sprites/spaceShip.png");
	explosionCTexture.loadFromFile("Sprites/explosions/Explosion.png");
	asteroidBigTexture.loadFromFile("Sprites/Asteroid.png");
	asteroidsmallTexture.loadFromFile("Sprites/AsteroidSmall.png");
	explosionSpaceShipTexture.loadFromFile("Sprites/explosions/ShipExplosion.png");

	spaceShipTexture.setSmooth(true);
	backgroundTexture.setSmooth(true);
	menuBackgroundTexture.setSmooth(true);


	shootBuffer.loadFromFile("Sounds/shoot.wav");
	explosionBuffer.loadFromFile("Sounds/explosion.wav");
	loseBuffer.loadFromFile("Sounds/LifeLost.wav");

	shootSound.setBuffer(shootBuffer);
	explosionSound.setBuffer(explosionBuffer);
	loseSound.setBuffer(loseBuffer);


	
	backgroundMusic.openFromFile("Music/BackgroundMusic.wav");
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(50);
	background.setTexture(backgroundTexture);
	menuBackground.setTexture(menuBackgroundTexture);
	menuMusic.openFromFile("Music/MenuMusic.wav");
	menuMusic.play();
	menuMusic.setVolume(50);

	gameOverMusic.openFromFile("Music/GameOverMusic.wav");
	victoryMusic.openFromFile("Music/VictoryMusic.wav");

	background.setTexture(backgroundTexture);

	missileTexture.loadFromFile("Sprites/Missile.png");

	spaceShip.animation = sSpaceShipNotMove;

	lifeText.setFont(font);
	lifeText.setCharacterSize(20);
	lifeText.setPosition(1220, windowHeight - 30);

	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setPosition(20, windowHeight - 30);

	screenText.setFont(font);
	screenText.setCharacterSize(60);
	screenText.setPosition(260, 250);


}



void Update()
{
	spaceShip.changeSpeed(sSpaceShipNotMove,sSpaceShipMove);
	spaceShip.picture.move(spaceShip.speedX*timeElapsed, spaceShip.speedY*timeElapsed);
	spaceShip.invincibleTime -= timeElapsed;
	if (spaceShip.invincibleTime < 0)spaceShip.isInvincible = false;
	spaceShip.angle = atan2f(mouseYPosition - spaceShip.picture.getPosition().y, mouseXPosition - spaceShip.picture.getPosition().x)+PI/2;
	spaceShip.picture.setRotation(spaceShip.angle * 180 / PI);

	if (spaceShip.picture.getPosition().x > windowWidth)
		spaceShip.picture.setPosition(0.0f, spaceShip.picture.getPosition().y);
	else if (spaceShip.picture.getPosition().x < 0.0f)
		spaceShip.picture.setPosition(windowWidth-0.1f, spaceShip.picture.getPosition().y);


	if (spaceShip.picture.getPosition().y > windowHeight)
		spaceShip.picture.setPosition(spaceShip.picture.getPosition().x, 0.1f);
	else if (spaceShip.picture.getPosition().y < 0.0f)
		spaceShip.picture.setPosition(spaceShip.picture.getPosition().x, windowHeight - 0.1f);

	colissionDetection(&spaceShip, getBucket(spaceShip.picture.getPosition()));

	nextshot += timeElapsed;



	for (int i = 0; i < missiles.size(); ++i)
	{
		missiles[i]->picture.move(std::cos(missiles[i]->angle)*missiles[i]->speed*timeElapsed, std::sin(missiles[i]->angle)*missiles[i]->speed*timeElapsed);

		if (missiles[i]->picture.getPosition().x > windowWidth || missiles[i]->picture.getPosition().x<0.0f || missiles[i]->picture.getPosition().y>windowHeight || missiles[i]->picture.getPosition().y < 0.0f)
		{
			delete missiles[i];
			missiles.erase(missiles.begin() + i);
		}
		else
		{
			colissionDetection(missiles[i], getBucket(missiles[i]->picture.getPosition()));
		}
	}



	for (int i = 0; i < asteroids.size(); ++i)
	{
		Vector2i currentBucket =getBucket(asteroids[i]->picture.getPosition());

		asteroids[i]->picture.move(std::cos(asteroids[i]->angle)*asteroids[i]->speed*timeElapsed, std::sin(asteroids[i]->angle)*asteroids[i]->speed*timeElapsed);
		colissionDetection(asteroids[i], getBucket(asteroids[i]->picture.getPosition()));

		if (asteroids[i]->picture.getPosition().x > windowWidth)
			asteroids[i]->picture.setPosition(0.0f, asteroids[i]->picture.getPosition().y);
		else if (asteroids[i]->picture.getPosition().x < 0.0f)
			asteroids[i]->picture.setPosition(windowWidth - 0.1f, asteroids[i]->picture.getPosition().y);


		if (asteroids[i]->picture.getPosition().y > windowHeight)
			asteroids[i]->picture.setPosition(asteroids[i]->picture.getPosition().x, 0.1f);
		else if (asteroids[i]->picture.getPosition().y < 0.0f)
			asteroids[i]->picture.setPosition(asteroids[i]->picture.getPosition().x, windowHeight - 0.1f);

		Vector2i newBucket = getBucket(asteroids[i]->picture.getPosition());
		if (currentBucket != newBucket)
		{
			addToBucket(newBucket, asteroids[i]);
			removeFromBucket(currentBucket, asteroids[i]);
		}

	}

	if (asteroids.size() <= 0)
	{
		victory = true;
		resetGame = true;

		std::stringstream ss;
		ss << "Level Cleared!!!\n Score:" << score << "\n Press SPACE for next Level";
		background.setColor(Color(182, 128, 230, 255));
		backgroundMusic.stop();   
		if (!vm)
		{
			victoryMusic.play();
			victoryMusic.setVolume(50);
		}
		vm = true;
		screenText.setString(ss.str());

	}

	std::stringstream scoreString;
	std::stringstream lifeString;
	lifeString << "Life: " << life;
	lifeText.setString(lifeString.str());
	scoreString << "Score: " << score;
	scoreText.setString(scoreString.str());
}

void Render()
{
	window.clear(Color::Black);
	window.draw(background);

	if (gameRunning)
	{
		
		spaceShip.animation.update();

		spaceShip.animation.sprite.setPosition(spaceShip.picture.getPosition().x, spaceShip.picture.getPosition().y);
		spaceShip.animation.sprite.setRotation(spaceShip.angle * 180 / PI);
		window.draw(spaceShip.animation.sprite);



		for (int i = 0; i < missiles.size(); ++i)
		{
			window.draw(missiles[i]->picture);
		}
		for (int i = 0; i < asteroids.size(); ++i)
		{
			asteroids[i]->animation.update();
			asteroids[i]->animation.sprite.setPosition(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y);
			asteroids[i]->animation.sprite.setRotation(asteroids[i]->angle * 180 / PI);
			window.draw(asteroids[i]->animation.sprite);
		}
		for (int i = 0; i < explosions.size(); ++i)
		{
			explosions[i]->animation.update();
			if (explosions[i]->animation.isEnd())
			{
				delete explosions[i];
				explosions.erase(explosions.begin() + i);
			}
			else
			{
				explosions[i]->animation.sprite.setPosition(explosions[i]->picture.getPosition().x, explosions[i]->picture.getPosition().y);
				explosions[i]->animation.sprite.setRotation(explosions[i]->angle * 180 / PI);
				window.draw(explosions[i]->animation.sprite);
			}
		}
		window.draw(lifeText);
		window.draw(screenText);
		window.draw(scoreText);
		window.display();
	}
	if (!gameRunning)
	{
		window.clear(Color::Black);
		window.draw(menuBackground);
		menu.draw(window);
		window.display();
	}


}

void HandleInput()
{
	Event event;
	while (window.pollEvent(event))
	{	
		if (!gameRunning)//Check if in menu
		{
			switch (event.type)
			{
			case Event::KeyReleased:
				switch (event.key.code)
				{
				case Keyboard::Up:
					menu.MoveUp();
					break;

				case Keyboard::Down:
					menu.MoveDown();
					break;

				case Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 1:
						std::cout << "Play has been pressed" << std::endl;
						gameRunning = true;
						menuMusic.stop();
						backgroundMusic.play();
						backgroundMusic.setLoop(true);
						backgroundMusic.setVolume(50);
						loadLevel(0);
						break;

					case 2:
						window.close();
						break;
					}
					break;
				}
				break;

			case Event::Closed:
				window.close();
				break;
			}
		}
		if (gameRunning) 
		{

			if (event.type == Event::Closed)
			{
				window.close();
				for (int i = 0; i < missiles.size(); i++)
				{
					delete missiles[i];
					missiles[i] = nullptr;
				}

				for (int i = 0; i < asteroids.size(); i++)
				{
					delete asteroids[i];
					asteroids[i] = nullptr;
				}

				for (int i = 0; i < explosions.size(); i++)
				{
					delete explosions[i];
					explosions[i] = nullptr;
				}
				missiles.clear();
				asteroids.clear();
				explosions.clear();
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Left || event.key.code == Keyboard::A))
			{
				spaceShip.moveleft = true;
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Right || event.key.code == Keyboard::D))
			{
				spaceShip.moveright = true;
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Up || event.key.code == Keyboard::W))
			{
				spaceShip.moveup = true;
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Down || event.key.code == Keyboard::S))
			{
				spaceShip.movedown = true;
			}

			else if (event.type == Event::KeyReleased && (event.key.code == Keyboard::Left || event.key.code == Keyboard::A))
			{
				spaceShip.moveleft = false;
			}
			else if (event.type == Event::KeyReleased && (event.key.code == Keyboard::Right || event.key.code == Keyboard::D))
			{
				spaceShip.moveright = false;
			}
			else if (event.type == Event::KeyReleased && (event.key.code == Keyboard::Up || event.key.code == Keyboard::W))
			{
				spaceShip.moveup = false;
			}
			else if (event.type == Event::KeyReleased && (event.key.code == Keyboard::Down || event.key.code == Keyboard::S))
			{
				spaceShip.movedown = false;
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Space))
			{
				if (spaceShipCrashed)
				{
					spaceShipCrashed = false;
					spaceShip.setPosition(windowWidth / 2, windowHeight / 2);
					spaceShip.speedX = 0.0f;
					spaceShip.speedY = 0.0f;
				}
				if(resetGame)
				{	
					vm = false;
					resetGame = false;
					victoryMusic.stop();
					gom = false;
					gameOverMusic.stop();
					backgroundMusic.play();
					backgroundMusic.setLoop(true);
					backgroundMusic.setVolume(50);
					background.setColor(Color(255, 255, 255, 255));
					if (victory)
					{
						level++;
						loadLevel(level);
						victory = false;
					}
					else
						loadLevel(0);
				}

				screenText.setString("");
			}
			
			else if (event.type == Event::MouseMoved && !resetGame && !victory)
			{
				if (Mouse::getPosition(window).x < windowWidth && Mouse::getPosition(window).x  > 0.0f)
				{
					mouseXPosition = Mouse::getPosition(window).x;
				}

				if (Mouse::getPosition(window).y < windowHeight && Mouse::getPosition(window).y  > 0.0f)
				{
					mouseYPosition = Mouse::getPosition(window).y;
				}
			}
			else if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Escape))
			{
				gameRunning = false;
				spaceShipCrashed = false;
				resetGame = false;
				victory = false;
				vm = false;
				gom = false;
				backgroundMusic.stop();
				menuMusic.play();
			}
		}
	}


	if (gameRunning && !resetGame && !victory)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (nextshot > shotDuration)
			{
				Missile* missilePointer = new Missile(spaceShip.picture.getPosition().x, spaceShip.picture.getPosition().y, 10);
				missilePointer->picture.setTexture(&missileTexture);
				missilePointer->angle = spaceShip.angle - PI / 2;
				missilePointer->picture.setRotation((missilePointer->angle + PI / 2) * 180 / PI);
				missiles.push_back(missilePointer);

				shootSound.play();

				nextshot = 0.0f;
			}
		}
	}

}

void loadLevel(int level)
{
	spaceShip.setPosition(windowWidth/2,windowHeight/2);
	spaceShip.speedX = 0.0f;
	spaceShip.speedY = 0.0f;
	spaceShip.isInvincible = true;
	spaceShip.invincibleTime = 2.0f;
	if (!victory)
	{
		score = 0;
		life = 3;
		level = 0;
	}
	for (int i = 0; i < asteroids.size(); ++i)
	{
		delete asteroids[i];
		asteroids[i] = nullptr;
	}
	asteroids.clear();

	int asteroidCount;
	asteroidCount = 10 + (level * 3);

	for (int i = 0; i<asteroidCount; i++)
	{

		int x, y;
		do
		{
			x = rand() % windowWidth;
			y = rand() % windowHeight;

		} while (x<windowWidth/2+25&&x>windowWidth/2-25&&y<windowHeight+25&&y>windowHeight-25);

		Asteroid* asteroidPointer = new Asteroid(x,y, 25);
		asteroidPointer->angle = static_cast<float>(rand())/static_cast<float>(RAND_MAX)*2*PI;
		asteroidPointer->hp = 3;
		asteroidPointer->speed = asteroidPointer->speed + level * 50;
		asteroidPointer->animation = sAsteroidBig;
		asteroids.push_back(asteroidPointer);
		addToBucket(getBucket(asteroidPointer->picture.getPosition()), asteroidPointer);
	}
}

Vector2i getBucket(Vector2f position)
{
	int col = int(position.x / BUCKET_WIDTH);
	if (col < 0)
		col = 0;
	else if (col >= COLUMNS)
		col = COLUMNS - 1;

	int row = int(position.y / BUCKET_HEIGHT);
	if (row < 0)
		row = 0;
	else if (row >= ROWS)
		row = ROWS - 1;

	return Vector2i(col, row);
}


void addToBucket(Vector2i b,	Asteroid* asteroidObject)
{
	grid[b.x][b.y].push_back(asteroidObject);
}

void removeFromBucket(Vector2i b, Asteroid* asteroidObject)
{
	std::vector<Asteroid*> & v = grid[b.x][b.y];

	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i] == asteroidObject)
		{
			v.erase(v.begin() + i);
			break;
		}
	}
}

void colissionDetection(GameObject* gameObject, Vector2i b)
{
	int left = std::max(b.x - 1, 0);
	int right = std::min(b.x + 1, COLUMNS - 1);
	int top = std::max(b.y - 1, 0);
	int bot = std::min(b.y + 1, ROWS - 1);
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (gameObject->checkCollisionWith(asteroids[i]))
		{
			if (gameObject->name == "missile")
			{
				for (int i = 0; i < missiles.size(); i++)
				{
					if (missiles[i] == gameObject)
					{
						delete gameObject;
						missiles.erase(missiles.begin() + i);
						break;
					}
				}
				asteroids[i]->hp = asteroids[i]->hp - 1;
				if (asteroids[i]->hp <= 0)
				{

					if (asteroids[i]->picture.getRadius() > 20)
					{
						Asteroid* asteroidPointer1 = new Asteroid(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y, 15);
						asteroidPointer1->angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * PI;
						asteroidPointer1->hp = 1;
						asteroidPointer1->animation = sAsteroidSmall;
						asteroids.push_back(asteroidPointer1);

						Asteroid* asteroidPointer2 = new Asteroid(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y, 15);
						asteroidPointer2->angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * PI;
						asteroidPointer2->hp = 1;
						asteroidPointer2->animation = sAsteroidSmall;
						asteroids.push_back(asteroidPointer2);

					}

					GameObject* explosion = new GameObject;
					explosion->picture.setPosition(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y);
					explosion->animation = sExplosionAsteroids;
					explosions.push_back(explosion);

					explosionSound.play();

					score += 100;
					delete asteroids[i];
					asteroids.erase(asteroids.begin() + i);
					break;
				}
			}
			else if (gameObject->name == "asteroid")
			{

				if (gameObject != asteroids[i])
				{
					float collisionAngle = atan2f(gameObject->picture.getPosition().y - asteroids[i]->picture.getPosition().y, gameObject->picture.getPosition().x - asteroids[i]->picture.getPosition().x);
					gameObject->angle = collisionAngle;
					asteroids[i]->angle = collisionAngle + PI;
				}

			}
			else if (gameObject->name == "spaceShip")
			{
				if (spaceShip.invincibleTime < 0.0f)
				{
					GameObject* explosion = new GameObject;
					explosion->picture.setPosition(spaceShip.picture.getPosition().x, spaceShip.picture.getPosition().y);
					explosion->animation = sExplosionSpaceShip;
					explosions.push_back(explosion);
					
					loseSound.play();
					spaceShipCrashed = true;
					spaceShip.isInvincible = true;
					spaceShip.invincibleTime = 2.0f;
					life--;
					spaceShip.setPosition(-1,-1);

					if (life <= 0)
					{
						screenText.setString("Score:" + std::to_string(score)+"\n	Space ships Depleated  game over \n press 'SPACE' to return\n or Esc to quit");
						background.setColor(Color(230, 128, 182, 255));
						backgroundMusic.stop();
						if (!gom)
						{
							gameOverMusic.play();
							gameOverMusic.setVolume(50);
						}
						gom = true;
						resetGame = true;
						victory = false;
					}
					else
					{
						screenText.setString("Space ship crashed \n press 'SPACE to continue'");
					}
				}
			}

		}
		

	}
			
}
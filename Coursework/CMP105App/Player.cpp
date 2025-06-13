#include "Player.h"

Player::Player()
{
	sealTexture.loadFromFile("gfx/debugTestSprites/happySeal.png"); //Doesn't exist yet
	setTexture(&sealTexture);
	setSize(sf::Vector2f(100, 100));
	setPosition(0,0);
	setMass(20);
	//setFillColor(sf::Color::Red);
}

Player::~Player()
{

}

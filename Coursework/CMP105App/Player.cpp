#include "Player.h"

Player::Player(sf::Vector2f pos, sf::Vector2f size, float mass) : CreatureObject(pos, size, mass)
{
	//setSize(sf::Vector2f(100, 100));
	//setPosition(0,0);
	//setMass(20);
	//setFillColor(sf::Color::Red);
}

Player::~Player()
{

}

void Player::lightAttack()
{
}

void Player::heavyAttack()
{
}

void Player::dodge()
{
}

void Player::parry()
{
}

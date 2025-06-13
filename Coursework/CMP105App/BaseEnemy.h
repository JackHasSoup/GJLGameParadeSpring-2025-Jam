#pragma once
#include "Framework/PhysicsObject.h"

class BaseEnemy : public PhysicsObject
{
public:
	BaseEnemy() {};
	BaseEnemy(sf::Vector2f pos, sf::Vector2f size, float mass);
	~BaseEnemy() {};

	void trackPlayer(PhysicsObject* player);
protected:
	sf::Vector2f direction, directionNorm;
	float speed = 200.f;
	float attackRange = 120.f; //distance from the player at which the enemy stops approaching and starts attacking
};
#pragma once
#include "Framework/PhysicsObject.h"
class CreatureObject : public PhysicsObject
{
public:
	CreatureObject(sf::Vector2f pos, sf::Vector2f size, float mass);
	CreatureObject() {};//default constructor
	~CreatureObject() {};

	virtual void lightAttack() = 0;
	virtual void heavyAttack() = 0;
	virtual void dodge() = 0;
	virtual void parry() = 0;

protected:
};


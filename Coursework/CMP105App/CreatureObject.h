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

	virtual void damage(float d) { health -= d; };
	virtual float getHealth() { return health; };
	virtual void restoreHealth() { health = maxHealth; };

	virtual void setSpeed(float s) { speed = s; };
	virtual float getSpeed() { return speed; };
protected:
	float health, maxHealth;
	float speed;
	float cooldown, maxCooldown;
};


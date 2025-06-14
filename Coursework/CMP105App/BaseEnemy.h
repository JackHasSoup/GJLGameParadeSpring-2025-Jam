#pragma once
#include "CreatureObject.h"
#include "BufferedCommand.h"
#include <vector>

class BaseEnemy : public CreatureObject
{
public:
	BaseEnemy() {};
	BaseEnemy(sf::Vector2f pos, sf::Vector2f size, float mass);
	~BaseEnemy() {};

	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt);
	void damage(float d) override;
protected:
	sf::Vector2f direction, directionNorm;
	float speed = 200.f;
	float attackRange = 120.f*120.f; //distance from the player at which the enemy stops approaching and starts attacking
	int actionBufferIndex = 0; //current index of the action copy
	float maxCooldown = 0.75f;
	float cooldown = maxCooldown;
};
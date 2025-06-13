#pragma once
#include "Framework/PhysicsObject.h"
#include "BufferedCommand.h"
#include <vector>

class BaseEnemy : public PhysicsObject
{
public:
	BaseEnemy() {};
	BaseEnemy(sf::Vector2f pos, sf::Vector2f size, float mass);
	~BaseEnemy() {};

	void trackPlayer(PhysicsObject* player, std::vector<BufferedCommand*> actionBuffer, float dt);
protected:
	sf::Vector2f direction, directionNorm;
	float speed = 200.f;
	float attackRange = 120.f*120.f; //distance from the player at which the enemy stops approaching and starts attacking
	int actionBufferIndex = 0; //current index of the action copy
	static constexpr float maxCooldown = 0.75f;
	float cooldown = maxCooldown;
};
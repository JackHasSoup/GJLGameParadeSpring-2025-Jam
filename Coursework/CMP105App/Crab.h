#pragma once
#include "BaseEnemy.h"

class Crab : public BaseEnemy
{
public : 
	Crab() {};
	Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction);
	~Crab() {};

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt);

private : 
	sf::Vector2f vecToPlayer, vecToProjPointNorm, vecToProjPoint;
	float heightDiff;
};


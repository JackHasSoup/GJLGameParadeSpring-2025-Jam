#pragma once
#include "CreatureObject.h"
#include "BufferedCommand.h"
#include "Framework/Animation.h"
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

	virtual void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt);
	void damage(float d) override;
protected:
	sf::Vector2f direction, directionNorm;
	int actionBufferIndex = 0; //current index of the action copy
	int howBloody = 0;
	Action lastAction = Action::NONE;
};
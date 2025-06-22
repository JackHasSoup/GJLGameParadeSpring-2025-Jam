#pragma once
#include "BaseEnemy.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"
#include "Framework/AudioManager.h"

class Crab : public BaseEnemy
{
public : 
	Crab() {};
	Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction);
	~Crab() {};

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) override;
	
	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;

	void setDirection(sf::Vector2f d);
private : 
	sf::Vector2f vecToPlayer, vecToProjPointNorm, vecToProjPoint, movementAxis;
	double heightDiff, rota1, rota2;
	bool heavyAtkActive = false;
	float heavyAtkMaxDuration, heavyAtkDuration;

	//int howBloody = 0;
	Animation pinch[3]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;
};


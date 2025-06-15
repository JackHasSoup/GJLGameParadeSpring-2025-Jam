#pragma once
#include "BaseEnemy.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"


class Crab : public BaseEnemy
{
public : 
	Crab() {};
	Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction);
	~Crab() {};

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt);
	
	virtual void lightAttack(CreatureObject* player) /*override*/;
	//virtual void heavyAttack(CreatureObject* player) override;
	virtual void dodge() override;
	virtual void parry() override;

	virtual void damage(float d) override;
private : 
	sf::Vector2f vecToPlayer, vecToProjPointNorm, vecToProjPoint, movementAxis;
	double heightDiff, rota1, rota2;

	int howBloody = 0;
	Animation pinch[3]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;
};


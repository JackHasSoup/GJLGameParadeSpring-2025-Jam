#pragma once
#include "BaseEnemy.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"

class Jellyfish : public BaseEnemy
{
public:
	Jellyfish() {};
	Jellyfish(sf::Vector2f pos, sf::Vector2f size, float mass);
	~Jellyfish() {};

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) override;

	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;
private:
	sf::Vector2f vecToPlayer, movementVec/*, vecToProjPointNorm, vecToProjPoint, movementAxis*/;
	//double heightDiff, rota1, rota2;
	bool heavyAtkActive = false;


	//int howBloody = 0;
	Animation zap[3]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;
};


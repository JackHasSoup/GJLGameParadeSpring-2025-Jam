#pragma once
#include "BaseEnemy.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"

class Walrus : public BaseEnemy
{

public:
	Walrus() {};
	Walrus(sf::Vector2f pos, sf::Vector2f size, float mass);
	~Walrus() {};

	void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) override;

	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;
private:
	sf::Vector2f vecToPlayer, movementVec/*, vecToProjPointNorm, vecToProjPoint, movementAxis*/;
	//double heightDiff, rota1, rota2;
	bool lightAttackActive, heavyAttackActive;
	int animFrame = 0;
	float animTimeElapsed = 0.f;
	sf::FloatRect lightAttackBox, heavyAttackBox;
	float lightAttackMaxDuration, lightAttackDuration, heavyAttackMaxDuration, heavyAttackDuration, heavyAttackSpeedMult;


	//int howBloody = 0;
	Animation crunch[4]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;

};


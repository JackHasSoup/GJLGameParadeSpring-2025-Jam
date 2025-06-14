#pragma once
#include "CreatureObject.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"

class Player : public CreatureObject
{
public:
	Player() {};
	Player(sf::Vector2f pos, sf::Vector2f size, float mass);
	~Player();

	virtual void update(float dt) override;

	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;

	/*float getHealth();
	void setHealth();
	void attack1();
	void attack2();
	void attack3();*/
	//probably add more attacks and rename them
	virtual void damage(float d) override;
protected:
	
	int howBloody = 0;
	Animation slap[3]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;
};


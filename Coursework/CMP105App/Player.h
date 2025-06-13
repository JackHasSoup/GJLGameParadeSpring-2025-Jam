#pragma once
#include "CreatureObject.h"
#include "Framework/AssetManager.h"

class Player : public CreatureObject
{
public:
	Player(sf::Vector2f pos, sf::Vector2f size, float mass);
	~Player();

	virtual void lightAttack() override;
	virtual void heavyAttack() override;
	virtual void dodge() override;
	virtual void parry() override;

	/*float getHealth();
	void setHealth();
	void attack1();
	void attack2();
	void attack3();*/
	//probably add more attacks and rename them


protected:
	float health;
	sf::Texture* sealTexture;
};


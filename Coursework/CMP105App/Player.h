#pragma once
#include "Framework/PhysicsObject.h"

class Player : public PhysicsObject
{
public:
	Player();
	~Player();
	/*float getHealth();
	void setHealth();
	void attack1();
	void attack2();
	void attack3();*/
	//probably add more attacks and rename them


protected:
	float health;
	sf::Texture sealTexture;
};


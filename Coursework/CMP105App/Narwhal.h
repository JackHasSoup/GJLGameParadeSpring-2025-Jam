#pragma once
#include "BaseEnemy.h"

class Narwhal : public BaseEnemy
{
public:
	Narwhal() {};
	Narwhal(sf::Vector2f pos, sf::Vector2f size, float mass);
	~Narwhal() {};

	virtual void lightAttack(std::vector<CreatureObject*> creatures) override;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) override;
	virtual void dodge() override;
	virtual void parry() override;

	virtual void update(float dt) override;
	virtual void trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) override;

	virtual void damage(float d) override;
private:
	Animation light[3];
	Animation heavy[3]; //also used for dodge
	Animation parryAnim[3];

	bool heavyHit = false;
	CreatureObject* player = nullptr; //the player object, used for heavy attack hit detection
};


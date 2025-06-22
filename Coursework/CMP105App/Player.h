#pragma once
#include "CreatureObject.h"
#include "Framework/Animation.h"
#include "Framework/AssetManager.h"
#include "Framework/AudioManager.h"

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

	virtual void jumpAnim(float dt);
	virtual void actualHeavyAttack(std::vector<CreatureObject*> creatures);

	/*float getHealth();
	void setHealth();
	void attack1();
	void attack2();
	void attack3();*/
	//probably add more attacks and rename them
	virtual void damage(float d) override;
	virtual void restoreHealth() override { health = maxHealth; howBloody = 0; };
	virtual void setJumpTime(float t) { jumpTime = t; };

	virtual void setHowBloody(int h) { howBloody = h; };
	virtual int getHowBloody() { return howBloody; };
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
	sf::RectangleShape jumpClone;
	float jumpTime = 0.f, jumpLength = 0.65f, jumpHeight = 75.f;
	int howBloody = 0;
	Animation slap[3]; //1-normal, 2-bloody, 3-very bloody (normal, side, down, up)
	Action lastAction = Action::NONE;

	std::vector<CreatureObject*> creaturesTemp;
	float invincibleTime = 0.f;

};


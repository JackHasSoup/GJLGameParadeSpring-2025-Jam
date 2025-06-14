#pragma once
#include "Framework/PhysicsObject.h"
enum class Action{NONE, LIGHT,HEAVY,DODGE,PARRY};
class CreatureObject : public PhysicsObject
{
public:
	CreatureObject(sf::Vector2f pos, sf::Vector2f size, float mass);
	CreatureObject() {};//default constructor
	~CreatureObject() {};

	virtual void lightAttack(std::vector<CreatureObject*> creatures) = 0;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) = 0;
	virtual void dodge() = 0;
	virtual void parry() = 0;

	virtual void update(float dt) override;

	virtual void damage(float d) { health -= d; };
	virtual float getHealth() { return health; };
	virtual void restoreHealth() { health = maxHealth; };

	virtual void setSpeed(float s) { speed = s; };
	virtual float getSpeed() { return speed; };

	virtual void setCooldown(float c) { cooldown = c; };
	virtual float getCooldown() { return cooldown; };

	virtual void setMaxCooldown(float mc) { maxCooldown = mc; };
	virtual float getMaxCooldown() { return maxCooldown; };

	virtual void setLightAttackDamage(float d) { lightAttackDamage = d; };
	virtual float getLightAttackDamage() { return lightAttackDamage; };
	virtual void setHeavyAttackDamage(float d) { heavyAttackDamage = d; };
	virtual float getHeavyAttackDamage() { return heavyAttackDamage; };
	virtual void setLightAttackRange(float r) { lightAttackRange = r; };
	virtual float getLightAttackRange() { return lightAttackRange; };
	virtual void setHeavyAttackRange(float r) { heavyAttackRange = r; };
	virtual float getHeavyAttackRange() { return heavyAttackRange; };

protected:
	float health, maxHealth;
	float speed;
	float cooldown, maxCooldown;
	float lightAttackDamage, heavyAttackDamage;
	float lightAttackRange, heavyAttackRange; //range is the coeficient of the size of the attack check box for melee attacks, the projectile range for ranged attacks
};


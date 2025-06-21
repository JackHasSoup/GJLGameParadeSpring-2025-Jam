#pragma once
#include "Framework/PhysicsObject.h"
#include "Framework/AssetManager.h"
enum class Action{NONE, LIGHT,HEAVY,DODGE,PARRY};
class CreatureObject : public PhysicsObject
{
public:
	CreatureObject(sf::Vector2f pos, sf::Vector2f size, float mass);
	CreatureObject() { hitFlash = nullptr; };//default constructor
	~CreatureObject() {};

	virtual void lightAttack(std::vector<CreatureObject*> creatures) = 0;
	virtual void heavyAttack(std::vector<CreatureObject*> creatures) = 0;
	virtual void dodge() = 0;
	virtual void parry() = 0;

	virtual void update(float dt) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void damage(float d) { health -= d; hitEffectTimer = hitEffectTimerMax;};
	virtual void setHealth(float d) { health = d; };
	virtual float getHealth() { return health; };
	virtual float getMaxHealth() { return maxHealth; };
	virtual void restoreHealth() { health = maxHealth; };

	virtual void setSpeed(float s) { speed = s; };
	virtual float getSpeed() { return speed; };

	virtual void setCooldown(float c) { cooldown = c; };
	virtual float getCooldown() { return cooldown; };

	virtual void setMaxCooldown(float mc) { maxCooldown = mc; };
	virtual float getMaxCooldown() { return maxCooldown; };

	virtual float getHitTimer() { return hitEffectTimer; };
	virtual void setHitTimer(float t) { hitEffectTimer = t; }
	virtual float getHitTimerMax() { return hitEffectTimerMax; };
	sf::Shader* getShader() { return hitFlash; };

	virtual sf::Vector2f getBasePos() { return basePosition; };
	virtual void setBasePos(sf::Vector2f p) { basePosition = p; }

	virtual void setLightAttackDamage(float d) { lightAttackDamage = d; };
	virtual float getLightAttackDamage() { return lightAttackDamage; };
	virtual void setHeavyAttackDamage(float d) { heavyAttackDamage = d; };
	virtual float getHeavyAttackDamage() { return heavyAttackDamage; };
	virtual void setLightAttackRange(float r) { lightAttackRange = r; };
	virtual float getLightAttackRange() { return lightAttackRange; };
	virtual void setHeavyAttackRange(float r) { heavyAttackRange = r; };
	virtual float getHeavyAttackRange() { return heavyAttackRange; };

protected:
	sf::Vector2f basePosition;
	float health, maxHealth;
	float speed;
	float cooldown, maxCooldown;
	float lightAttackDamage, heavyAttackDamage;
	float lightAttackRange, heavyAttackRange; //range is the coeficient of the size of the attack check box for melee attacks, the projectile range for ranged attacks
	float hitEffectTimer, hitEffectTimerMax;
	sf::Shader* hitFlash;
};


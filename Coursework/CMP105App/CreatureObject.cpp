#include "CreatureObject.h"

CreatureObject::CreatureObject(sf::Vector2f pos, sf::Vector2f size, float mass) : PhysicsObject(pos, size, mass)
{
	hitEffectTimerMax = 0.225f;
	hitEffectTimer = 0.f;

	hitFlash = AssetManager::getShader("flash");
	hitFlash->setUniform("timerMax", hitEffectTimerMax);

}

void CreatureObject::update(float dt)
{
	hitEffectTimer -= dt;

	PhysicsObject::update(dt);
}

void CreatureObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (hitEffectTimer > 0.f) {
		hitFlash->setUniform("timer", hitEffectTimer);
		PhysicsObject::draw(target, hitFlash);
	}
	else {
		PhysicsObject::draw(target, states);
	}
}

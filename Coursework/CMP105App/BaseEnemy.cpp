#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(sf::Vector2f pos, sf::Vector2f size, float mass) : CreatureObject(pos, size, mass)
{

}

void BaseEnemy::lightAttack(std::vector<CreatureObject*> creatures)
{
}

void BaseEnemy::heavyAttack(std::vector<CreatureObject*> creatures)
{
}

void BaseEnemy::dodge()
{
}

void BaseEnemy::parry()
{
}

void BaseEnemy::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	cooldown -= dt;
	direction = sf::Vector2f(player->getPosition().x - getPosition().x, player->getPosition().y - getPosition().y);
	directionNorm = VectorHelper::normalise(direction);

	const int size = actionBuffer.size();
	if (VectorHelper::magnitudeSqrd(direction) >= attackRange) {//compare sqr magnitude for performance reasons
		accelerate(directionNorm, speed);
	}
	else if(cooldown <= 0.f && size) {
		cooldown = maxCooldown;

		actionBuffer[actionBufferIndex]->execute(this, { player });
		actionBufferIndex = actionBufferIndex + 1 >= size ? 0 : actionBufferIndex + 1;
	}
}
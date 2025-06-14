#include "Crab.h"

Crab::Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction) : BaseEnemy(pos, size, mass)
{
	heightDiff = pos.y - (pos.x * direction.y);
	vecToProjPointNorm = VectorHelper::normalise(direction);
	
	speed = 500;
}

void Crab::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	CreatureObject::update(dt);

	vecToPlayer = player->getPosition() - getPosition();
	vecToProjPoint = (VectorHelper::dot(vecToPlayer, vecToProjPointNorm) / VectorHelper::dot(vecToProjPointNorm, vecToProjPointNorm)) * vecToProjPointNorm;

	accelerate(VectorHelper::normalise(vecToProjPoint) * speed);
}
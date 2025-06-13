#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(sf::Vector2f pos, sf::Vector2f size, float mass) : PhysicsObject(pos, size, mass)
{

}

void BaseEnemy::trackPlayer(PhysicsObject* player) {
	direction = sf::Vector2f(player->getPosition().x - getPosition().x, player->getPosition().y - getPosition().y);
	directionNorm = VectorHelper::normalise(direction);

	if (VectorHelper::magnitude(direction) >= attackRange) {
		accelerate(directionNorm, speed);
	}
}
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
	if (!isAlive())return;
	cooldown -= dt;
	/*direction = sf::Vector2f(player->getPosition().x - getPosition().x, player->getPosition().y - getPosition().y);
	directionNorm = VectorHelper::normalise(direction);*/

	const int size = actionBuffer.size();
	//if (VectorHelper::magnitudeSqrd(direction) >= attackRange) {//compare sqr magnitude for performance reasons
	//	accelerate(directionNorm, speed);
	//}
	if(cooldown <= 0.f && size) {
		actionBuffer[actionBufferIndex]->execute(this, { player });
		actionBufferIndex = actionBufferIndex + 1 >= size ? 0 : actionBufferIndex + 1;
		cooldown = maxCooldown;
	}
}

void BaseEnemy::damage(float d)
{
	CreatureObject::damage(d);
	if (health < 0) health = 0;
	if (health < maxHealth / 3) howBloody = 2; //very bloody
	else if (health < maxHealth / 2) howBloody = 1; //bloody
	else howBloody = 0; //normal

	if (health <= 0)
	{
		setAlive(false);
	}
}


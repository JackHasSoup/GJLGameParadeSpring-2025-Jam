#include "Crab.h"

Crab::Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("crabSheet");
	AssetManager::getTex("crabSheet")->loadFromFile("gfx/Crab/crabSheet.png");
	setTexture(AssetManager::getTex("crabSheet"));
	setFillColor(sf::Color::White);

	heightDiff = pos.y - (pos.x * direction.y);
	vecToProjPointNorm = VectorHelper::normalise(direction);
	
	rota1 = atan(direction.y / direction.x);
	rota2 = rota1 + 3.1415926;

	maxCooldown = 0.75f;
	cooldown = 0.f;
	speed = 350.f;
	health = 100.f;
	maxHealth = 100.f;
	lightAttackDamage = 10.f;
	heavyAttackDamage = 25.f;
	lightAttackRange = 1.f;
	heavyAttackDamage = 2.5f;

	setDrawType(drawType::RECT);

	auto cs = sf::ConvexShape(4);
	cs.setPoint(0, { 0.f, 0.f });
	cs.setPoint(1, { getSize().x, 0.f });
	cs.setPoint(2, { getSize().x, getSize().y });
	cs.setPoint(3, { 0.f, getSize().y });
	setCollisionShape(cs);

	for (int i = 0; i < 2; i++)
	{
		pinch[0].addFrame({ 0, 150 * i, 300, 150 });
		pinch[1].addFrame({ 300, 150 * i, 300, 150 });
		pinch[2].addFrame({ 600, 150 * i, 300, 150 });
	}
}

void Crab::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	CreatureObject::update(dt);

	vecToPlayer = player->getPosition() - getPosition();
	vecToProjPoint = (VectorHelper::dot(vecToPlayer, vecToProjPointNorm) / VectorHelper::dot(vecToProjPointNorm, vecToProjPointNorm)) * vecToProjPointNorm;

	if (player->getPosition().x > )

	accelerate(VectorHelper::normalise(vecToProjPoint) * speed);
}

void Crab::lightAttack(CreatureObject* player)
{
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	sf::FloatRect attackBox;
	if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	{
		attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
		//auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
		if (mPos.y < getPosition().y - getOrigin().y)
		{
			attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
			attackBox.top -= attackBox.height / 2.f;
		}
		else if (mPos.y > getPosition().y + getOrigin().y)
		{
		attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		attackBox.top += attackBox.height / 2.f;
		}
		else {
			attackBox.width *= lightAttackRange; //increase the width of the attack box for light attack
			if (pinch[howBloody].getFlipped()) //if the player is facing left
			{
				attackBox.left -= attackBox.width / 2.f;
			}
			else {
				attackBox.left += attackBox.width / 2.f;
			}
		}
		//check if the attack box intersects the creature's collision shape
		if (player->getCollisionShape().getGlobalBounds().intersects(attackBox))
		{
			player->damage(lightAttackDamage);
			std::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
		}
		else {
			std::cout << "plyr miss\n"; //missed
		}
	}
	else {
		std::cout << "return\n";
		return; //no valid frame, no attack
	}
	std::cout << "plyr light\n";
}

void Crab::heavyAttack(std::vector<CreatureObject*> creatures)
{
	//TEMP: heavy attack is just more powerfull light attack
	for (auto const& c : creatures)
	{
		//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
		sf::FloatRect attackBox;
		if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
		{
			attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
			auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
			if (mPos.y < getPosition().y - getOrigin().y)
			{
				attackBox.height *= heavyAttackRange; //increase the height of the attack box for heavy attack
				attackBox.top -= attackBox.height / 2.f;
			}
			else if (mPos.y > getPosition().y + getOrigin().y)
			{
				attackBox.height *= heavyAttackRange; //increase the height of the attack box for heavy attack
				attackBox.top += attackBox.height / 2.f;
			}
			else {
				attackBox.width *= heavyAttackRange; //increase the width of the attack box for heavy attack
				if (pinch[howBloody].getFlipped()) //if the player is facing left
				{
					attackBox.left -= attackBox.width / 2.f;
				}
				else {
					attackBox.left += attackBox.width / 2.f;
				}
			}

			//check if the attack box intersects the creature's collision shape
			if (c->getCollisionShape().getGlobalBounds().intersects(attackBox))
			{
				c->damage(heavyAttackDamage);
				std::cout << "plyr hit " << c->getPosition().x << ", " << c->getPosition().y << "\n";
				c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
			}
			else {
				std::cout << "plyr miss\n"; //missed
			}
		}
		else {
			return; //no valid frame, no attack
		}
	}
	std::cout << "plyr heavy\n";
	lastAction = Action::HEAVY;
}

void Crab::dodge()
{
	sf::Vector2f dir = (sf::Vector2f(0, 1) * (float)Input::isKeyDown(sf::Keyboard::S)) +
		(sf::Vector2f(0, -1) * (float)Input::isKeyDown(sf::Keyboard::W)) +
		(sf::Vector2f(-1, 0) * (float)Input::isKeyDown(sf::Keyboard::A)) +
		(sf::Vector2f(1, 0) * (float)Input::isKeyDown(sf::Keyboard::D));
	accelerate(dir, speed * speed);
	lastAction = Action::DODGE;
}

void Crab::parry()
{
	std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}

void Crab::damage(float d)
{
	health -= d;
	if (health < 0) health = 0;
	if (health < maxHealth / 3) howBloody = 3; //very bloody
	else if (health < maxHealth / 2) howBloody = 2; //bloody
	else howBloody = 1; //normal
}
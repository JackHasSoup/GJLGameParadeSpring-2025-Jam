#include "Player.h"

Player::Player(sf::Vector2f pos, sf::Vector2f size, float mass) : CreatureObject(pos, size, mass)
{
	//setSize(sf::Vector2f(100, 100));
	//setPosition(0,0);
	//setMass(20);
	//setFillColor(sf::Color::Red);
	AssetManager::registerNewTex("sealSlapSheet");
	AssetManager::getTex("sealSlapSheet")->loadFromFile("gfx/Seal/Slap.png");
	setTexture(AssetManager::getTex("sealSlapSheet"));
	setFillColor(sf::Color::White);

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

	for (int i = 0; i < 4; i++)
	{
		slap[0].addFrame({ 0, 215 * i, 253, 215 });
		slap[1].addFrame({ 253, 215 * i, 253, 215 });
		slap[2].addFrame({ 506, 215 * i, 253, 215 });
	}
}

Player::~Player()
{

}

void Player::update(float dt)
{
	CreatureObject::update(dt);

	if (cooldown <= 0) //not on cooldown, action not being performed
	{
		lastAction = Action::NONE;
	}

	auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
	switch (lastAction)
	{
	case Action::LIGHT:
	{
		if (mPos.y < getPosition().y - getOrigin().y)
		{
			slap[howBloody].setFrame(3);
		}
		else if (mPos.y > getPosition().y + getOrigin().y)
		{
			slap[howBloody].setFrame(2);
		}
		else {
			slap[howBloody].setFrame(1);
		}
	}
		break;
	case Action::HEAVY:
		break;
	case Action::DODGE:
		break;
	case Action::PARRY:
		break;
	default:
		slap[howBloody].setFrame(0); //regular seal
		break;
	}

	//if mouse position is left of seal position flip the animation
	if (VectorHelper::magnitudeSqrd(lastPos - getPosition()) < 0.01f) //if not moving use mouse
	{
		slap[howBloody].setFlipped(mPos.x < getPosition().x);
	}
	else {
		slap[howBloody].setFlipped(lastPos.x > getPosition().x);
	}
	
	setTextureRect(slap[howBloody].getCurrentFrame());
	std::cout << health << std::endl;
}

void Player::lightAttack(std::vector<CreatureObject*> creatures)
{
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	for (auto const& c : creatures)
	{
		//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
		sf::FloatRect attackBox;
		if (slap[howBloody].getCurrentFrame().width != 0) //if the frame is valid
		{
			attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());

			auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
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
				if (slap[howBloody].getFlipped()) //if the player is facing left
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
				c->damage(lightAttackDamage);
				std::cout << "plyr hit " << c->getPosition().x << ", " << c->getPosition().y << "\n";
				c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
			}
			else {
				std::cout << "plyr miss\n"; //missed
			}
		}
		else {
			std::cout << "return\n";
			return; //no valid frame, no attack
		}
	}
	std::cout << "plyr light\n";
}

void Player::heavyAttack(std::vector<CreatureObject*> creatures)
{
	//TEMP: heavy attack is just more powerfull light attack
	for (auto const& c : creatures)
	{
		//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
		sf::FloatRect attackBox;
		if (slap[howBloody].getCurrentFrame().width != 0) //if the frame is valid
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
				if (slap[howBloody].getFlipped()) //if the player is facing left
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

void Player::dodge()
{
	sf::Vector2f dir = (sf::Vector2f(0, 1) * (float)Input::isKeyDown(sf::Keyboard::S)) +
						(sf::Vector2f(0, -1) * (float)Input::isKeyDown(sf::Keyboard::W)) +
						(sf::Vector2f(-1, 0) * (float)Input::isKeyDown(sf::Keyboard::A)) +
						(sf::Vector2f(1, 0) * (float)Input::isKeyDown(sf::Keyboard::D));
	accelerate(dir, speed * speed);
	lastAction = Action::DODGE;
}

void Player::parry()
{
	std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}

void Player::damage(float d)
{
	health -= d;
	if (health < 0) health = 0;
	if (health < maxHealth / 3) howBloody = 2; //very bloody
	else if (health < maxHealth / 2) howBloody = 1; //bloody
	else howBloody = 0; //normal
}

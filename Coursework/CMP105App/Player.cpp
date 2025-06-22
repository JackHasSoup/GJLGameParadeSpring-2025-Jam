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
	speed = 600.f;
	health = 10.f;
	maxHealth = 10.f;
	lightAttackDamage = 0.5f;
	heavyAttackDamage = 1.5f;
	lightAttackRange = 1.f;
	heavyAttackRange = 1.75f;

	setDrawType(drawType::RECT_COL_LIGHTMASK);
	//50,36
	sf::CircleShape c = sf::CircleShape(size.x * 0.61f * 0.5f);//0.61 is the how much of the sprite is actually seal
	collisionShape = sf::ConvexShape(c.getPointCount() );
	for (int i = 0; i < c.getPointCount(); i++)
	{
		collisionShape.setPoint(i, c.getPoint(i) + sf::Vector2f(size.x * 0.198f, size.y * 0.167f)); //point + offset of seal from corner of grid
	}
	baseHull = collisionShape;

	for (int i = 0; i < 4; i++)
	{
		slap[0].addFrame({ 0, 215 * i, 253, 215 });
		slap[1].addFrame({ 253, 215 * i, 253, 215 });
		slap[2].addFrame({ 506, 215 * i, 253, 215 });
	}

	jumpClone = *dynamic_cast<sf::RectangleShape*>(this); //clone the player for jump animation, so it can be rotated without affecting the player
	jumpClone.setOrigin(getOrigin());

	AudioManager::createSound("damageTaken", "sfx/ouch2", 1.0f, false);
	AudioManager::createSound("slapSound", "sfx/slap", 1.0f, false);
	AudioManager::createSound("jumpAttack", "sfx/sealJumpAttack3.wav", 0.02f, false);
	AudioManager::createSound("dodge", "sfx/sealDodge", 1.0f, false);

	AudioManager::setMaxSoundVol(1000.f);

}

Player::~Player()
{

}

void Player::update(float dt)
{
	CreatureObject::update(dt);

	cooldown -= dt;
	invincibleTime -= dt; //decrease the invincible time

	if (cooldown <= 0) //not on cooldown, action not being performed
	{
		lastAction = Action::NONE;
	}

	auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
	switch (lastAction)
	{
	case Action::LIGHT:
	{
		if (slap[howBloody].getFrame() != 0) break; //only update if the player is not already attacking
		auto const dif = mPos - getPosition();
		if (abs(dif.y) > abs(dif.x))
		{
			if (dif.y < 0)
			{
				slap[howBloody].setFrame(3); //up
			}
			else {
				slap[howBloody].setFrame(2); //down
			}
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
	if (slap[howBloody].getFrame() == 0) //only flip if no animation happening
	{
		if (VectorHelper::magnitudeSqrd(lastPos - getPosition()) < 0.01f) //if not moving use mouse
		{
			slap[howBloody].setFlipped(mPos.x < getPosition().x);
		}
		else {
			slap[howBloody].setFlipped(lastPos.x > getPosition().x);
		}
	}
	
	setTextureRect(slap[howBloody].getCurrentFrame());
	jumpAnim(dt); //update the jump animation if the player is jumping
}

void Player::lightAttack(std::vector<CreatureObject*> creatures)
{
	AudioManager::getSound("slapSound")->playAt(getPosition());

	if (cooldown > 0) return; //if the player is on cooldown, don't attack
	lastAction = Action::LIGHT;
	setCooldown(maxCooldown);

	update(0.f); //update to set the correct frame for the attack
	for (auto const& c : creatures)
	{
		//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
		sf::FloatRect attackBox;
		if (slap[howBloody].getCurrentFrame().width != 0) //if the frame is valid
		{
			attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());

			auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
			auto const dif = mPos - getPosition();
			if (abs(dif.y) > abs(dif.x))
			{
				if (dif.y < 0)
				{
					attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
					attackBox.top -= attackBox.height / 2.f;
				}
				else {
					attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
					attackBox.top += attackBox.height / 2.f;
				}
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
				//std::cout << "plyr hit " << c->getPosition().x << ", " << c->getPosition().y << "\n";
				c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
			}
			else {
				//std::cout << "plyr miss\n"; //missed
			}
		}
		else {
			//std::cout << "return\n";
			return; //no valid frame, no attack
		}
	}
	//std::cout << "plyr light\n";
}

void Player::heavyAttack(std::vector<CreatureObject*> creatures)
{
	AudioManager::getSound("jumpAttack")->playAt(getPosition());

	if (cooldown > 0) return; //if the player is on cooldown, don't attack
	setCooldown(maxCooldown * 1.5f);//longer cooldown for heavy attack

	//don't do attack here, wait to land
	jumpTime = jumpLength; //reset the jump time for the jump animation
	creaturesTemp = creatures;
	//std::cout << "plyr heavy\n";
	lastAction = Action::HEAVY;
}

void Player::actualHeavyAttack(std::vector<CreatureObject*> creatures)
{

	for (auto const& c : creatures)
	{
		if (VectorHelper::magnitudeSqrd(c->getPosition() - getPosition()) < heavyAttackRange * heavyAttackRange * getSize().x * getSize().y) //check if the creature is within the heavy attack range
		{
			c->damage(heavyAttackDamage);
			//std::cout << "plyr hit " << c->getPosition().x << ", " << c->getPosition().y << "\n";
			c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
		}
		else {
			//std::cout << "plyr miss\n"; //missed
		}
	}
}

void Player::dodge()
{
	AudioManager::getSound("dodge")->playAt(getPosition());

	if (cooldown > 0) return; //if the player is on cooldown, don't attack
	setCooldown(maxCooldown * 0.75f); //shorter cooldown for dodge

	sf::Vector2f dir = (sf::Vector2f(0, 1) * (float)Input::isKeyDown(sf::Keyboard::S)) +
						(sf::Vector2f(0, -1) * (float)Input::isKeyDown(sf::Keyboard::W)) +
						(sf::Vector2f(-1, 0) * (float)Input::isKeyDown(sf::Keyboard::A)) +
						(sf::Vector2f(1, 0) * (float)Input::isKeyDown(sf::Keyboard::D));
	accelerate(dir, (speed * speed * 0.4f));
	lastAction = Action::DODGE;
}

void Player::parry()
{
	if (cooldown > 0) return; //if the player is on cooldown, don't attack
	setCooldown(maxCooldown * 0.35f); //shorter cooldown for parry

	//parry just gives some invincible time
	invincibleTime = 0.5f; //set the invincible time to 0.5 seconds
	//std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}

void Player::jumpAnim(float dt)
{
	if(jumpTime > 0)
	{
		float p = jumpTime / jumpLength;
		jumpClone.setRotation((slap[howBloody].getFlipped() ? 335.f : -335.f) * (p > 0.5f ? p*2.f : 1.f)); //rotate the player based on the jump time
		jumpClone.setPosition(
			getPosition() - sf::Vector2f(
				0, 
				jumpTime >= jumpLength * 0.5f ?
				sin(PI * p) * jumpHeight :
				jumpHeight * pow(2.f * (p), 5.f)
			));
		jumpClone.setTextureRect(slap[howBloody].getCurrentFrame()); //set the texture rect to the current frame of the slap animation

		if (jumpTime - dt < 0)
		{
			actualHeavyAttack(creaturesTemp); //perform the heavy attack when the player hits the floor
		}
	}
	jumpTime -= dt;
}

void Player::damage(float d)
{
	if (invincibleTime > 0) return; //if the player is invincible, don't take damage
	CreatureObject::damage(d);
	if (health < 0) health = 0;
	if (health < maxHealth / 3) howBloody = 2; //very bloody
	else if (health < maxHealth / 2) howBloody = 1; //bloody
	else howBloody = 0; //normal
	//std::cout << health << std::endl;
	AudioManager::getSound("damageTaken")->playAt(getPosition());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (jumpTime>0)
		target.draw(jumpClone, states);
	else 
		CreatureObject::draw(target, states);
}

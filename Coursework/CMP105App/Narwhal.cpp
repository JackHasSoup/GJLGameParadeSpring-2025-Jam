#include "Narwhal.h"

Narwhal::Narwhal(sf::Vector2f pos, sf::Vector2f size, float mass) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("NarwhalLight");
	AssetManager::getTex("NarwhalLight")->loadFromFile("gfx/Narwhal/LightAttack.png");
	setTexture(AssetManager::getTex("NarwhalLight"));

	AssetManager::registerNewTex("NarwhalHeavy");
	AssetManager::getTex("NarwhalHeavy")->loadFromFile("gfx/Narwhal/HeavyAttack.png");

	AssetManager::registerNewTex("NarwhalParry");
	AssetManager::getTex("NarwhalParry")->loadFromFile("gfx/Narwhal/NarwhalParry.png");

	setFillColor(sf::Color::White);
	setDrawType(drawType::RECT_COL_LIGHTMASK);

	std::vector<sf::Vector2i> points = {
		{-10,2},
        {-7,-2},
        {0,4},
        {5,6},
        {11,5},
        {15,8},
        {16,7},
        {13,3},
        {15,0},
        {14,-5},
        {9,-7},
        {2,-8},
        {-6,-7},
        {-11,-7},
	};

	collisionShape.setPointCount(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		collisionShape.setPoint(i, sf::Vector2f(
			((float)points[i].x / 32.f) * size.x * 0.85,
			((float)points[i].y / 22.f) * -size.y * 0.85f //-y because the collision coords is upside down
		) + getOrigin());
	}
	baseHull = collisionShape;

	setRotationLock(true);
	setAlive(true);

	health = 7.5f;
	maxHealth = 12.5f;
	speed = 250.f;
	cooldown = 0.f;
	maxCooldown = 2.5f;
	lightAttackDamage = 0.5f;
	heavyAttackDamage = 3.5f;
	lightAttackRange = 0.5f; //50% of the size of the attack check box
	heavyAttackRange = 1.0f; //100% of the size of the attack check box

	lightAttackRange = getSize().x; //the distance the narwhal will stop approaching the player

	for (int i = 0; i < 4; i++)
	{
		light[0].addFrame({ 0, 256 * i, 256, 256 });
		light[1].addFrame({ 256, 256 * i, 256, 256 });
		light[2].addFrame({ 512, 256 * i, 256, 256 });
	}
	for (int i = 0; i < 3; i++)
	{
		heavy[i].addFrame({256* i, 0, 256, 256});
		parryAnim[i] = heavy[i]; //parry uses the same frames as heavy attack
	}
	light[howBloody].setFrame(0);
	setTextureRect(light[howBloody].getCurrentFrame());
}

void Narwhal::lightAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return; //if the narwhal is on cooldown, don't attack
	setTexture(AssetManager::getTex("NarwhalLight"));
	lastAction = Action::LIGHT;
	cooldown = maxCooldown;
	update(0.f); //update to set the correct frame for the attack

	auto* c = creatures[0]; //get the player, the first (and only) creature in the vector
	//check if the creature intersects a box sent out from narwhal's look direction on attack (look direction being the direction the narwhal is facing like in update getting the frame for light attack)
	sf::FloatRect attackBox;
	if (light[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	{
		attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
		auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
		auto const dif = mPos - getPosition();
		if (abs(dif.y) > abs(dif.x))
		{
			if (dif.y < 0)
			{
				attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
			}
			else {
				attackBox.height *= lightAttackRange * 0.5f; //decrease the height of the attack box for light attack
			}
		}
		else {
			if (dif.x < 0)
			{
				attackBox.width *= lightAttackRange * 0.5f; //decrease the width of the attack box for light attack
			}
			else {
				attackBox.width *= lightAttackRange; //increase the width of the attack box for light attack
			}
		}
		if (attackBox.intersects(c->getCollisionShape().getGlobalBounds())) //check if the creature is within the light attack range
		{
			c->damage(lightAttackDamage);
			c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
		}
	}
}

void Narwhal::heavyAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return; //if the narwhal is on cooldown, don't attack
	setTexture(AssetManager::getTex("NarwhalHeavy"));
	lastAction = Action::HEAVY;
	cooldown = maxCooldown * 1.5f; //longer cooldown for heavy attack
	update(0.f); //update to set the correct frame for the attack

	accelerate((creatures[0]->getPosition() - getPosition()) * speed * 2.f); //charge towards the player
}

void Narwhal::dodge()
{
	if (cooldown > 0) return; //if the narwhal is on cooldown, don't attack
	setTexture(AssetManager::getTex("NarwhalHeavy"));
	lastAction = Action::DODGE;
	cooldown = maxCooldown * 1.15f; //longer cooldown for dodge
	update(0.f); //update to set the correct frame for the attack

	accelerate(getVelocity(1.f), -speed * speed); //charge in opposite direction of current movement
}

void Narwhal::parry()
{
	if (cooldown > 0) return; //if the narwhal is on cooldown, don't attack
	setTexture(AssetManager::getTex("NarwhalParry"));
	lastAction = Action::PARRY;
	cooldown = maxCooldown * 0.25f; //short parry length
	update(0.f); //update to set the correct frame for the action

	//this is all that has to be done, the parry animation will play and the narwhal will not take damage while it is parrying
}

void Narwhal::update(float dt)
{
	BaseEnemy::update(dt);

	if (cooldown <= 0) //not on cooldown, action not being performed
	{
		lastAction = Action::NONE;
	}

	//switch for animation frame
	switch (lastAction)
	{
	case Action::LIGHT:
	{
		float p = (cooldown * 2.f) / maxCooldown; //only animate for half the cooldown
		if (p > 1.f) break;
		if (p > 0.67f) { light[howBloody].setFrame(1); }else 
		if (p > 0.33f) { light[howBloody].setFrame(2); }else
		if (p > 0.165) { light[howBloody].setFrame(3); }
		else{light[howBloody].setFrame(0);}
		setTextureRect(light[howBloody].getCurrentFrame());
		break;
	}
	case Action::HEAVY:
		setTextureRect(heavy[howBloody].getCurrentFrame());
		break;
	default:
		light[howBloody].setFrame(0); //regular narwhal
		setTextureRect(light[howBloody].getCurrentFrame());
		break;
	}

	
}

void Narwhal::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt)
{
	BaseEnemy::trackPlayer(player, actionBuffer, dt);
	if (!isAlive()) return;

	//look at player direction
	bool flip = player->getPosition().x < getPosition().x;
	flip = lastAction == Action::DODGE ? !flip : flip; //if dodging, flip the narwhal to face the opposite direction
	light[howBloody].setFlipped(flip);
	heavy[howBloody].setFlipped(flip);

	if (VectorHelper::magnitudeSqrd(player->getPosition() - getPosition()) >= lightAttackRange* lightAttackRange)
	{
		accelerate(VectorHelper::normalise(player->getPosition() - getPosition()), speed);//no dt, handled by physics anyway
	}

}

void Narwhal::damage(float d)
{
	if (lastAction == Action::PARRY) return; //if the narwhal is parrying, don't take damage
	BaseEnemy::damage(d);
}

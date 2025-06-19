#include "Crab.h"

Crab::Crab(sf::Vector2f pos, sf::Vector2f size, float mass, sf::Vector2f direction) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("crabSheet");
	AssetManager::getTex("crabSheet")->loadFromFile("gfx/Crab/crabSheetFinalFinal.png");
	setTexture(AssetManager::getTex("crabSheet"));
	setFillColor(sf::Color::White);

	(direction.x == 0) ? heightDiff = 1 : heightDiff = pos.y - (pos.x * (direction.y / direction.x));
	vecToProjPointNorm = VectorHelper::normalise(direction);
	movementAxis = direction;

	rota1 = atan(direction.y / direction.x) * 180 / 3.1415926;
	rota2 = (atan(direction.y / direction.x) + 3.1415926) * 180 / 3.1415926;

	maxCooldown = 4.f;
	cooldown = 4.f;
	speed = 350.f;
	health = 2.5f;
	maxHealth = 2.5f;

	lightAttackDamage = 1.f;
	heavyAttackDamage = 2.5f;
	lightAttackRange = 1.f;
	heavyAttackRange = 1.5f;
	heavyAtkMaxDuration = 0.5f;
	heavyAtkDuration = 0.5f;

	setDrawType(drawType::RECT_COL_LIGHTMASK);

	std::vector<sf::Vector2f> p ={
	{ 0.f,7.f },
	{ 4.f, 7.f },
	{ 7.f, 6.f },
	{ 9.f, 5.f },
	{ 13.f, 3.f },
	{ 19.f, 0.f },
	{ 14.f, -6.f },
	{ 5.f, -6.f },
	{ -5.f, -6.f },
	{ -14.f, -6.f },
	{ -19.f, 0.f },
	{ -13.f, 3.f },
	{ -9.f, 5.f },
	{ -7.f, 6.f },
	{ -4.f, 7.f },
	};
	collisionShape.setPointCount(p.size());
	for (int i = 0; i < p.size(); i ++ )
	{
		collisionShape.setPoint(i, sf::Vector2f(
			((float)p[i].x / 38.f) * size.x,
			((float)p[i].y / 14.f) * -size.y //-y because the collision coords is upside down
		) + getOrigin());
	}
	baseHull = collisionShape;

	for (int i = 0; i < 4; i++)
	{
		pinch[0].addFrame({ 0, 150 * i, 300, 150 });
		pinch[1].addFrame({ 300, 150 * i, 300, 150 });
		pinch[2].addFrame({ 600, 150 * i, 300, 150 });
	}
	//pinch[0].setFrame(0);
}

void Crab::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	//CreatureObject::update(dt);
	BaseEnemy::trackPlayer(player, actionBuffer, dt);

	//std::cout << howBloody << std::endl;
	if (cooldown <= 0) //not on cooldown, action not being performed
	{
		lastAction = Action::NONE;
		//std::cout << "last action none" << std::endl;
	}
	switch (lastAction)
	{
	case Action::LIGHT:
	{
		pinch[howBloody].setFrame(1);
	}
	break;
	case Action::HEAVY:
		pinch[howBloody].setFrame(2);
		break;
	case Action::DODGE:
		pinch[howBloody].setFrame(0);
		break;
	case Action::PARRY:
		pinch[howBloody].setFrame(3);
		std::cout << "parried" << std::endl;
		break;
	default:
		pinch[howBloody].setFrame(0); //regular crab
		break;
	}
	
	vecToPlayer = player->getPosition() - getPosition();
	vecToProjPoint = (VectorHelper::dot(vecToPlayer, vecToProjPointNorm) / VectorHelper::dot(vecToProjPointNorm, vecToProjPointNorm)) * vecToProjPointNorm;

	if (abs(getPosition().y - (getPosition().x * (movementAxis.y / movementAxis.x) + heightDiff)) > 1) {
		heightDiff = getPosition().y - (getPosition().x * (movementAxis.y / movementAxis.x));
		//std::cout << "change axis" << std::endl;
		//std::cout << getPosition().x << " ; " << getPosition().y << " ; " << ((movementAxis.y / movementAxis.x)) << std::endl;
	}

	if (player->getPosition().y > (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff) {
		//std::cout << "above axis" << std::endl;
		setRotation(rota1);
	}
	else if (player->getPosition().y < (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff) {
		//std::cout << "above axis" << player->getPosition().y << " ; " << (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff << " ; " << heightDiff << std::endl;
		setRotation(rota2);
	}

	if (heavyAtkActive == true) {
		heavyAtkDuration -= dt;
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < (((getSize().x * getSize().y) / 2.f) * heavyAttackRange))
		{
			player->damage(heavyAttackDamage);
			//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it

			heavyAtkActive = false;
			heavyAtkDuration = heavyAtkMaxDuration;
		}
		if (heavyAtkDuration <= 0) {
			heavyAtkDuration = heavyAtkMaxDuration;
			heavyAtkActive = false;
		}
	}
	accelerate(VectorHelper::normalise(vecToProjPoint) * speed);
	if (cooldown >= maxCooldown/15.f) {
		// regular crab sprite if not able to attack
		pinch[howBloody].setFrame(0);
	}
	setTextureRect(pinch[howBloody].getCurrentFrame());
	//std::cout << heightDiff << std::endl;
}

void Crab::lightAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return;
	cooldown = maxCooldown;
	CreatureObject* player = creatures[0];
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	//pinch[howBloody].setFrame(1);
	sf::FloatRect attackBox;
	if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	{
		//check if the attack box intersects the creature's collision shape
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < (((getSize().x * getSize().y) / 2.f) * lightAttackRange))
		{
			player->damage(lightAttackDamage);
			//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
		}
		else {
			//std::cout << VectorHelper::magnitudeSqrd(vecToPlayer) << std::endl; //missed
		}
	}
	else {
		//std::cout << "return\n";
		return; //no valid frame, no attack
	}
	//std::cout << "plyr light\n";
}

void Crab::heavyAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return;
	cooldown = maxCooldown * 2.5f;

	CreatureObject* player = creatures[0];
	
	lastAction = Action::HEAVY;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	pinch[howBloody].setFrame(2);

	accelerate(VectorHelper::normalise(vecToPlayer) * speed * speed);

	heavyAtkActive = true;

	sf::FloatRect attackBox;
	//if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	//{
	//	//check if the attack box intersects the creature's collision shape
	//	if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAttackRange)
	//	{
	//		player->damage(heavyAttackDamage);
	//		//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
	//		player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
	//	}
	//	else {
	//		//std::cout << VectorHelper::magnitudeSqrd(vecToPlayer) << std::endl; //missed
	//	}
	//}
}

void Crab::dodge()
{
	if (cooldown > 0) return;
	cooldown = maxCooldown * 1.15f;

	accelerate(VectorHelper::normalise(vecToProjPoint) * speed * speed);
	lastAction = Action::DODGE;
	//std::cout << "dodge" << std::endl;
}

void Crab::parry()
{
	if (cooldown > 0) return;
	cooldown = maxCooldown * 0.25f;
	//std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}
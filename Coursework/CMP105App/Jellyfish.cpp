#include "Jellyfish.h"

Jellyfish::Jellyfish(sf::Vector2f pos, sf::Vector2f size, float mass) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("jellySheet");
	AssetManager::getTex("jellySheet")->loadFromFile("gfx/Jellyfish/Jellyfish.png");
	setTexture(AssetManager::getTex("jellySheet"));
	setFillColor(sf::Color::White);

	/*(direction.x == 0) ? heightDiff = 1 : heightDiff = pos.y - (pos.x * (direction.y / direction.x));
	vecToProjPointNorm = VectorHelper::normalise(direction);
	movementAxis = direction;

	rota1 = atan(direction.y / direction.x) * 180 / 3.1415926;
	rota2 = (atan(direction.y / direction.x) + 3.1415926) * 180 / 3.1415926;*/

	maxCooldown = 3.f;
	cooldown = 3.f;
	speed = 200.f;
	health = 1.f;
	maxHealth = 1.f;

	lightAttackDamage = 1.f;
	heavyAttackDamage = 2.f;
	lightAttackRange = 7500.f;
	heavyAttackRange = 12000.f;

	movementVec = sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1);

	setDrawType(drawType::RECT_COL_LIGHTMASK);

	std::vector<sf::Vector2i> p = {
		{0,11},
		{6,10},
		{10,7},
		{14,3},
		{14,1},
		{12,-2},
		{12,-8},
		{11,-11},
		{8,-8},
		{6,-10},
		{4,-10},
		{3,-6},
		{2,-10},
		{0,-10},
		{-1,-8},
		{-3,-10},
		{-4,-10},
		{-6,-7},
		{-10,-11},
		{-12,-10},
		{-11,1},
		{-12,5},
		{-7,9},
		{-4,10},
	};

	collisionShape.setPointCount(p.size());
	for (int i = 0; i < p.size(); i++)
	{
		collisionShape.setPoint(i, sf::Vector2f(
			((float)p[i].x / 28.f) * size.x * 0.32f,
			((float)p[i].y / 22.f) * -size.y * 0.32f //-y because the collision coords is upside down
		) + sf::Vector2f(getOrigin().x, getOrigin().y * 0.80f));
	}
	baseHull = collisionShape;

	for (int i = 0; i < 2; i++) {
		zap[i].addFrame({ 600 * i, 1200, 600, 600 });
		zap[i].addFrame({ 600 * i, 600, 600, 600 });
		zap[i].addFrame({ 600 * i, 0, 600, 600 });
	}
	
	//pinch[0].setFrame(0);

	AudioManager::createSound("jellyfishLight", "sfx/jellyfish", 1.0f, false);
	AudioManager::createSound("jellyfishHeavy", "sfx/jellyfishExtra", 1.0f, false);
	AudioManager::createSound("dodge", "sfx/sealDodge", 1.0f, false);



}

void Jellyfish::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	//CreatureObject::update(dt);
	BaseEnemy::trackPlayer(player, actionBuffer, dt);
	//std::cout << howBloody << std::endl;
	
	animTimeElapsed += dt * 15;
	(sin(animTimeElapsed) >= 0) ? animFrame = 0 : animFrame = 1;
	zap[animFrame].setFrame(0);
	setTextureRect(zap[animFrame].getCurrentFrame());
	//std::cout << animFrame << std::endl;
	
	float p = cooldown / maxCooldown;

	if (cooldown <= 0) //not on cooldown, action not being performed
	{
		lastAction = Action::NONE;
		//std::cout << "last action none" << std::endl;
	}
	switch (lastAction)
	{
	case Action::LIGHT:
	{
		if (p < 0.75f) { zap[animFrame].setFrame(0); }
		else { zap[animFrame].setFrame(1); };
		setTextureRect(zap[animFrame].getCurrentFrame());
	}
	break;
	case Action::HEAVY:
		if (p < 0.75f) { zap[animFrame].setFrame(0); heavyAtkActive = false; }
		else { zap[animFrame].setFrame(2); };
		setTextureRect(zap[animFrame].getCurrentFrame());
		break;
	case Action::DODGE:
		zap[animFrame].setFrame(0);
		break;
	case Action::PARRY:
		zap[animFrame].setFrame(3);
		//std::cout << "parried" << std::endl;
		break;
	default:
		zap[animFrame].setFrame(0); //regular crab
		break;
	}

	movementVec += sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1);
	//std::cout << movementVec.x << " ; " << movementVec.y <<  std::endl;

	vecToPlayer = player->getPosition() - getPosition();
	//vecToProjPoint = (VectorHelper::dot(vecToPlayer, vecToProjPointNorm) / VectorHelper::dot(vecToProjPointNorm, vecToProjPointNorm)) * vecToProjPointNorm;

	//if (abs(getPosition().y - (getPosition().x * (movementAxis.y / movementAxis.x) + heightDiff)) > 1) {
	//	heightDiff = getPosition().y - (getPosition().x * (movementAxis.y / movementAxis.x));
	//	//std::cout << "change axis" << std::endl;
	//	//std::cout << getPosition().x << " ; " << getPosition().y << " ; " << ((movementAxis.y / movementAxis.x)) << std::endl;
	//}

	//if (player->getPosition().y > (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff) {
	//	//std::cout << "above axis" << std::endl;
	//	setRotation(rota1);
	//}
	//else if (player->getPosition().y < (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff) {
	//	//std::cout << "above axis" << player->getPosition().y << " ; " << (movementAxis.y / movementAxis.x) * player->getPosition().x + heightDiff << " ; " << heightDiff << std::endl;
	//	setRotation(rota2);
	//}

	if (heavyAtkActive == true) {
		heavyAttackRange -= dt;
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAttackRange)
		{
			player->damage(heavyAttackDamage);
			//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it

			heavyAtkActive = false;
		}
	}

	accelerate(VectorHelper::normalise(movementVec) * speed);
	//setTextureRect(zap[0].getCurrentFrame());
	//std::cout << heightDiff << std::endl;
}

void Jellyfish::lightAttack(std::vector<CreatureObject*> creatures)
{
	AudioManager::getSound("jellyfishLight")->playAt(getPosition());

	if (cooldown > 0) return;
	
	movementVec = { 0.f, 0.f };
	cooldown = maxCooldown;
	CreatureObject* player = creatures[0];
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	//pinch[howBloody].setFrame(1);
	sf::FloatRect attackBox;
	if (zap[animFrame].getCurrentFrame().width != 0) //if the frame is valid
	{
		//check if the attack box intersects the creature's collision shape
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < lightAttackRange)
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

void Jellyfish::heavyAttack(std::vector<CreatureObject*> creatures)
{
	AudioManager::getSound("jellyfishLight")->playAt(getPosition()); //These are both meant to be here, not just the heavy one
	AudioManager::getSound("jellyfishHeavy")->playAt(getPosition());

	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	cooldown = maxCooldown * 2.5f;

	CreatureObject* player = creatures[0];

	lastAction = Action::HEAVY;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	zap[animFrame].setFrame(2);

	//accelerate(VectorHelper::normalise(vecToPlayer) * speed * speed);

	heavyAtkActive = true;

	sf::FloatRect attackBox;
	if (zap[animFrame].getCurrentFrame().width != 0) //if the frame is valid
	{
		//check if the attack box intersects the creature's collision shape
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAttackRange)
		{
			player->damage(heavyAttackDamage);
			//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
		}
		else {
			std::cout << VectorHelper::magnitudeSqrd(vecToPlayer) << std::endl; //missed
		}
	}
}

void Jellyfish::dodge()
{
	AudioManager::getSound("dodge")->playAt(getPosition());

	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	//cooldown = maxCooldown * 1.15f;

	//accelerate(VectorHelper::normalise(vecToProjPoint) * speed * speed);
	lastAction = Action::DODGE;
	//std::cout << "dodge" << std::endl;
}

void Jellyfish::parry()
{
	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	//cooldown = maxCooldown * 0.25f;
	//std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}
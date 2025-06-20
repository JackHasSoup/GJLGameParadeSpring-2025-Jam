#include "Walrus.h"

Walrus::Walrus(sf::Vector2f pos, sf::Vector2f size, float mass) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("WalrusSheet");
	AssetManager::getTex("WalrusSheet")->loadFromFile("gfx/Walrus/WalrusSheet.png");
	setTexture(AssetManager::getTex("WalrusSheet"));
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

	lightAttackDamage = 2.f;
	heavyAttackDamage = 3.5f;
	lightAttackRange = 7500.f;
	heavyAttackRange = 12000.f;

	lightAttackActive = false;
	heavyAttackActive = false;
	lightAttackMaxDuration, lightAttackDuration = 0.5f;
	heavyAttackMaxDuration, heavyAttackDuration = 1.f;


	//movementVec = sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1);

	setDrawType(drawType::RECT);

	auto cs = sf::ConvexShape(4);
	cs.setPoint(0, { 95.f, 90.f });
	cs.setPoint(1, { getSize().x - 110.f, 200.f });
	cs.setPoint(2, { getSize().x - 110.f, getSize().y - 140.f });
	cs.setPoint(3, { 95.f, getSize().y - 120.f });
	setCollisionShape(cs);

	for (int i = 0; i < 4; i++) {
		crunch[i].addFrame({ 200 * i, 0, 200, 200 });
		crunch[i].addFrame({ 200 * i, 200, 200, 200 });
		crunch[i].addFrame({ 200 * i, 400, 200, 200 });
		//crunch[i].addFrame({ 600, 200 * i, 200, 200 });
	}

	//pinch[0].setFrame(0);
}

void Walrus::trackPlayer(CreatureObject* player, std::vector<BufferedCommand*> actionBuffer, float dt) {
	//CreatureObject::update(dt);
	BaseEnemy::trackPlayer(player, actionBuffer, dt);
	//std::cout << howBloody << std::endl;

	animTimeElapsed += dt * 7.5f;
	if (sin(animTimeElapsed) <= -0.7f) {
		animFrame = 0;
	}
	else if (sin(animTimeElapsed) <= 0.7f) {
		animFrame = 1;
	}
	else if (sin(animTimeElapsed) <= 1.f) {
		animFrame = 2;
	}
	else if (sin(animTimeElapsed) <= 1.f) {
		animFrame = 3;
	}
	//(sin(animTimeElapsed) >= 0) ? animFrame = 0 : animFrame = 1;
	crunch[animFrame].setFrame(0);
	setTextureRect(crunch[animFrame].getCurrentFrame());
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
		if (p < 0.75f) { crunch[animFrame].setFrame(0); }
		else if (p < 0.85f) { crunch[animFrame].setFrame(2); }
		else { crunch[animFrame].setFrame(1); };
		setTextureRect(crunch[animFrame].getCurrentFrame());
	}
	break;
	case Action::HEAVY:
		if (p < 0.75f) { crunch[animFrame].setFrame(0); }
		else { crunch[animFrame].setFrame(2); };
		setTextureRect(crunch[animFrame].getCurrentFrame());
		break;
	case Action::DODGE:
		crunch[animFrame].setFrame(0);
		break;
	case Action::PARRY:
		crunch[animFrame].setFrame(3);
		std::cout << "parried" << std::endl;
		break;
	default:
		crunch[animFrame].setFrame(0); //regular crab
		break;
	}

	//movementVec += sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1);
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

	if (vecToPlayer.x > 0) { 
		crunch[animFrame].setFlipped(true); 
	}
	else if (vecToPlayer.x <= 0) { 
		crunch[animFrame].setFlipped(false); 
	}
	
	/*lightAttackBox = sf::FloatRect(getPosition() - getOrigin(), getSize() / 3.f);
	if (player->getCollisionShape().getGlobalBounds().intersects(lightAttackBox))
	{
		std::cout << "in range" << std::endl;
	}*/

	if (lightAttackActive == true) {
		lightAttackBox = sf::FloatRect(getPosition() - getOrigin(), getSize() / 2.f);

		if (lightAttackDuration <= 0) {
			lightAttackActive = false;
		}
		else {
			lightAttackDuration -= dt;
		}

		if (crunch[animFrame].getFlipped()) 
		{
			lightAttackBox.left = getPosition().x;
		}
		else {
			lightAttackBox.left = getPosition().x - (getSize().x / 1.5f);
		}

		if (player->getCollisionShape().getGlobalBounds().intersects(lightAttackBox))
		{
			player->damage(lightAttackDamage);
			std::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
			lightAttackActive = false;
		}
		
	}

	if (heavyAttackActive == true) {
		//heavyAttackRange -= dt;
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAttackRange)
		{
			player->damage(heavyAttackDamage);
			//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
			player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it

			heavyAttackActive = false;
		}
	}

	accelerate(VectorHelper::normalise(vecToPlayer) * speed);
	//setTextureRect(zap[0].getCurrentFrame());
	//std::cout << heightDiff << std::endl;
}

void Walrus::lightAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return;

	cooldown = maxCooldown;
	CreatureObject* player = creatures[0];
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	//pinch[howBloody].setFrame(1);
	sf::FloatRect attackBox;

	accelerate(VectorHelper::normalise(vecToPlayer) * speed * speed / 1.1f);
	lightAttackActive = true;
	//if (crunch[animFrame].getCurrentFrame().width != 0) //if the frame is valid
	//{
	//	attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());

	//	//attackBox.width *= lightAttackRange; //increase the width of the attack box for light attack
	//	if (crunch[animFrame].getFlipped()) //if the player is facing left
	//	{
	//		attackBox.left += attackBox.width / 2.f;
	//		std::cout << "og : " << attackBox.left << " ; new : " << getPosition().x << std::endl;
	//	}
	//	else {
	//		attackBox.left -= attackBox.width / 2.f;
	//		std::cout << "og : " << attackBox.left << " ; new : " << getPosition().x - getSize().x << std::endl;
	//	}

	//	//std::cout << " ahh : " << attackBox.left << std::endl;

	//	//check if the attack box intersects the creature's collision shape
	//	if (player->getCollisionShape().getGlobalBounds().intersects(attackBox))
	//	{
	//		player->damage(lightAttackDamage);
	//		std::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
	//		player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
	//	}
	//	else {
	//		//std::cout << "plyr miss\n"; //missed
	//	}
	//}
	//else {
	//	//std::cout << "return\n";
	//	return; //no valid frame, no attack
	//}
//std::cout << "plyr light\n";
	//if (crunch[animFrame].getCurrentFrame().width != 0) //if the frame is valid
	//{
	//	//check if the attack box intersects the creature's collision shape
	//	if (VectorHelper::magnitudeSqrd(vecToPlayer) < lightAttackRange)
	//	{
	//		player->damage(lightAttackDamage);
	//		//d::cout << "plyr hit " << player->getPosition().x << ", " << player->getPosition().y << "\n";
	//		player->setCooldown(player->getMaxCooldown()); //reset the cooldown of the creature, to stun it
	//	}
	//	else {
	//		std::cout << VectorHelper::magnitudeSqrd(vecToPlayer) << std::endl; //missed
	//	}
	//}
	//else {
	//	//std::cout << "return\n";
	//	return; //no valid frame, no attack
	//}
	//std::cout << "plyr light\n";
}

void Walrus::heavyAttack(std::vector<CreatureObject*> creatures)
{
	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	cooldown = maxCooldown * 2.5f;

	CreatureObject* player = creatures[0];

	lastAction = Action::HEAVY;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	crunch[animFrame].setFrame(2);

	//accelerate(VectorHelper::normalise(vecToPlayer) * speed * speed);

	heavyAttackActive = true;

	sf::FloatRect attackBox;
	if (crunch[animFrame].getCurrentFrame().width != 0) //if the frame is valid
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

void Walrus::dodge()
{
	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	//cooldown = maxCooldown * 1.15f;

	//accelerate(VectorHelper::normalise(vecToProjPoint) * speed * speed);
	lastAction = Action::DODGE;
	//std::cout << "dodge" << std::endl;
}

void Walrus::parry()
{
	if (cooldown > 0) return;

	movementVec = { 0.f, 0.f };
	//cooldown = maxCooldown * 0.25f;
	//std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}
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
	rota2 = (atan(direction.y / direction.x) + 3.1415926) * 180 / 3.1415926 ;

	maxCooldown = 0.75f;
	cooldown = 0.f;
	speed = 350.f;
	health = 2.5f;
	maxHealth = 2.5f;
	
	lightAttackDamage = 1.f;
	heavyAttackDamage = 2.5f;
	lightAtkRadius = 3600.f;
	heavyAtkRadius = 3600.f;
	heavyAtkMaxDuration = 0.25f;
	heavyAtkDuration = 0.25f;

	setDrawType(drawType::RECT);

	auto cs = sf::ConvexShape(4);
	cs.setPoint(0, { 20.f, 15.f });
	cs.setPoint(1, { getSize().x - 20.f, 15.f });
	cs.setPoint(2, { getSize().x - 20.f, getSize().y - 15.f });
	cs.setPoint(3, { 20.f, getSize().y - 15.f });
	setCollisionShape(cs);

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
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAtkRadius)
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
	setTextureRect(pinch[howBloody].getCurrentFrame());
	//std::cout << heightDiff << std::endl;
}

void Crab::lightAttack(std::vector<CreatureObject*> creatures)
{
	CreatureObject* player = creatures[0];
	lastAction = Action::LIGHT;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	//pinch[howBloody].setFrame(1);
	sf::FloatRect attackBox;
	if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	{
		//attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
		////auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
		//if (getRotation() == rota1) {
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top += attackBox.height / 2.f;
		//}
		//else if (getRotation() == rota2) {
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top -= attackBox.height / 2.f;
		//}
		//if (mPos.y < getPosition().y - getOrigin().y)
		//{
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top -= attackBox.height / 2.f;
		//}
		//else if (mPos.y > getPosition().y + getOrigin().y)
		//{
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top += attackBox.height / 2.f;
		//}
		//else {
		//	attackBox.width *= lightAttackRange; //increase the width of the attack box for light attack
		//	if (pinch[howBloody].getFlipped()) //if the player is facing left
		//	{
		//		attackBox.left -= attackBox.width / 2.f;
		//	}
		//	else {
		//		attackBox.left += attackBox.width / 2.f;
		//	}
		//}
		//check if the attack box intersects the creature's collision shape
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < lightAtkRadius)
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
	CreatureObject* player = creatures[0];
	
	lastAction = Action::HEAVY;
	update(0.f); //update to set the correct frame for the attack
	//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
	pinch[howBloody].setFrame(2);

	accelerate(VectorHelper::normalise(vecToPlayer) * speed * speed);

	heavyAtkActive = true;

	sf::FloatRect attackBox;
	if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
	{
		//attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
		////auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
		//if (getRotation() == rota1) {
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top += attackBox.height / 2.f;
		//}
		//else if (getRotation() == rota2) {
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top -= attackBox.height / 2.f;
		//}
		//if (mPos.y < getPosition().y - getOrigin().y)
		//{
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top -= attackBox.height / 2.f;
		//}
		//else if (mPos.y > getPosition().y + getOrigin().y)
		//{
		//	attackBox.height *= lightAttackRange; //increase the height of the attack box for light attack
		//	attackBox.top += attackBox.height / 2.f;
		//}
		//else {
		//	attackBox.width *= lightAttackRange; //increase the width of the attack box for light attack
		//	if (pinch[howBloody].getFlipped()) //if the player is facing left
		//	{
		//		attackBox.left -= attackBox.width / 2.f;
		//	}
		//	else {
		//		attackBox.left += attackBox.width / 2.f;
		//	}
		//}
		//check if the attack box intersects the creature's collision shape
		if (VectorHelper::magnitudeSqrd(vecToPlayer) < heavyAtkRadius)
		{
			player->damage(heavyAttackDamage);
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
	//	//TEMP: heavy attack is just more powerfull light attack
//	
// 
// for (auto const& c : creatures)
//	{
//		//check if the creature intersects a box sent out from players look direction on attack (look direction being the direction the player is facing like in update getting the frame for slap)
//		sf::FloatRect attackBox;
//		if (pinch[howBloody].getCurrentFrame().width != 0) //if the frame is valid
//		{
//			attackBox = sf::FloatRect(getPosition() - getOrigin(), getSize());
//			auto const mPos = GameState::getRenderTarget()->mapPixelToCoords(Input::getIntMousePos());
//			if (mPos.y < getPosition().y - getOrigin().y)
//			{
//				attackBox.height *= heavyAttackRange; //increase the height of the attack box for heavy attack
//				attackBox.top -= attackBox.height / 2.f;
//			}
//			else if (mPos.y > getPosition().y + getOrigin().y)
//			{
//				attackBox.height *= heavyAttackRange; //increase the height of the attack box for heavy attack
//				attackBox.top += attackBox.height / 2.f;
//			}
//			else {
//				attackBox.width *= heavyAttackRange; //increase the width of the attack box for heavy attack
//				if (pinch[howBloody].getFlipped()) //if the player is facing left
//				{
//					attackBox.left -= attackBox.width / 2.f;
//				}
//				else {
//					attackBox.left += attackBox.width / 2.f;
//				}
//			}
//
//			//check if the attack box intersects the creature's collision shape
//			if (c->getCollisionShape().getGlobalBounds().intersects(attackBox))
//			{
//				c->damage(heavyAttackDamage);
//				std::cout << "plyr hit " << c->getPosition().x << ", " << c->getPosition().y << "\n";
//				c->setCooldown(c->getMaxCooldown()); //reset the cooldown of the creature, to stun it
//			}
//			else {
//				std::cout << "plyr miss\n"; //missed
//			}
//		}
//		else {
//			return; //no valid frame, no attack
//		}
//	}
//	std::cout << "plyr heavy\n";
//	lastAction = Action::HEAVY;
}

void Crab::dodge()
{
	/*sf::Vector2f dir = (sf::Vector2f(0, 1) * (float)Input::isKeyDown(sf::Keyboard::S)) +
		(sf::Vector2f(0, -1) * (float)Input::isKeyDown(sf::Keyboard::W)) +
		(sf::Vector2f(-1, 0) * (float)Input::isKeyDown(sf::Keyboard::A)) +
		(sf::Vector2f(1, 0) * (float)Input::isKeyDown(sf::Keyboard::D));*/
	accelerate(VectorHelper::normalise(vecToProjPoint) * speed * speed);
	lastAction = Action::DODGE;
	//std::cout << "dodge" << std::endl;
}

void Crab::parry()
{
	//std::cout << "plyr parry\n";
	lastAction = Action::PARRY;
}
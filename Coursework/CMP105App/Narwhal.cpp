#include "Narwhal.h"

Narwhal::Narwhal(sf::Vector2f pos, sf::Vector2f size, float mass) : BaseEnemy(pos, size, mass)
{
	AssetManager::registerNewTex("NarwhalLight");
	AssetManager::getTex("NarwhalLight")->loadFromFile("gfx/Narwhal/LightAttack.png");
	setTexture(AssetManager::getTex("NarwhalLight"));
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
	speed = 100.f;
	cooldown = 0.f;
	maxCooldown = 1.5f;
	lightAttackDamage = 0.5f;
	heavyAttackDamage = 3.5f;
	lightAttackRange = 0.5f; //50% of the size of the attack check box
	heavyAttackRange = 1.0f; //100% of the size of the attack check box

	for (int i = 0; i < 4; i++)
	{
		light[0].addFrame({ 0, 256 * i, 256, 256 });
		light[1].addFrame({ 256, 256 * i, 256, 256 });
		light[2].addFrame({ 512, 256 * i, 256, 256 });
	}

	light[howBloody].setFrame(0);
	setTextureRect(light[howBloody].getCurrentFrame());
}

void Narwhal::lightAttack(std::vector<CreatureObject*> creatures)
{
}

void Narwhal::heavyAttack(std::vector<CreatureObject*> creatures)
{
}

void Narwhal::dodge()
{
}

void Narwhal::parry()
{
}

void Narwhal::update(float dt)
{
	BaseEnemy::update(dt);
}

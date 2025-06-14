#include "Player.h"

Player::Player(sf::Vector2f pos, sf::Vector2f size, float mass) : CreatureObject(pos, size, mass)
{
	//setSize(sf::Vector2f(100, 100));
	//setPosition(0,0);
	//setMass(20);
	//setFillColor(sf::Color::Red);
	AssetManager::registerNewTex("happySeal");
	AssetManager::getTex("happySeal")->loadFromFile("gfx/debugTestSprites/happySeal.png"); //Doesn't exist yet
	setTexture(AssetManager::getTex("happySeal"));
	setFillColor(sf::Color::White);

	maxCooldown = 0.75f;
	cooldown = 0.f;
	speed = 350.f;
	health = 100.f;
	maxHealth = 100.f;

	setDrawType(drawType::RECT);

	auto cs = sf::ConvexShape(4);
	cs.setPoint(0, { 0.f, 0.f });
	cs.setPoint(1, { getSize().x, 0.f });
	cs.setPoint(2, { getSize().x, getSize().y });
	cs.setPoint(3, { 0.f, getSize().y });
	setCollisionShape(cs);
}

Player::~Player()
{

}

void Player::lightAttack()
{
	std::cout << "plyr light\n";
}

void Player::heavyAttack()
{
	std::cout << "plyr heavy\n";
}

void Player::dodge()
{
	sf::Vector2f dir = (sf::Vector2f(0, 1) * (float)Input::isKeyDown(sf::Keyboard::S)) +
						(sf::Vector2f(0, -1) * (float)Input::isKeyDown(sf::Keyboard::W)) +
						(sf::Vector2f(-1, 0) * (float)Input::isKeyDown(sf::Keyboard::A)) +
						(sf::Vector2f(1, 0) * (float)Input::isKeyDown(sf::Keyboard::D));
	accelerate(dir, speed * speed);
}

void Player::parry()
{
	std::cout << "plyr parry\n";
}

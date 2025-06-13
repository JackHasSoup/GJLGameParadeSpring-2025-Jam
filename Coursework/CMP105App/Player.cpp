#include "Player.h"

Player::Player()
{
	//sealTexture.loadFromFile("gfx/sealTexture.png"); //Doesn't exist yet
	setSize(sf::Vector2f(50, 50));
	setPosition(100, 100);

	//availableActions = {
	//	new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 0.f, -35000.f }); }),//up
	//	new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 0.f, 35000.f }); }),//down
	//	new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ -35000.f, 0.f }); }),//left
	//	new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 35000.f, 0.f }); })//right
	//};
}

Player::~Player()
{

}

	

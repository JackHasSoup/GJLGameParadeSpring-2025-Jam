#include "BaseLevel.h"

BaseLevel::BaseLevel()
{
	window = nullptr;
}

BaseLevel::BaseLevel(sf::RenderTarget* hwnd) : Scene(hwnd)
{

	// Variable initalisation
	enemyCount = 0;
	bgColor = sf::Color(130, 112, 148);

	// Player
	player = Player(midWin, { 75.f, 75.f }, 20.f);

	floor.setSize(sf::Vector2f{ window->getSize() });
	floor.setOrigin(floor.getSize() / 2.f);
	floor.setPosition(midWin);

	floor.setTextureRect(sf::IntRect(0, 0, floor.getSize().x, floor.getSize().y));

	door = PhysicsObject((midWin - sf::Vector2f{ 0,400 }), sf::Vector2f{ 200.f,300.f }, 100);
	door.setFillColor(sf::Color::White);

	doorLightI = lighter.addLight(door.getPosition() - sf::Vector2f(-23.f, 70.f), 100.f, sf::Color::Red);
	doorLight = Light(door.getPosition() - sf::Vector2f(0, 40.f), 50.f, sf::Color::Red);

	physMan.registerObj(&player, false);
	physMan.registerObj(&door, true);

	commander.addHeld(sf::Keyboard::W, new GenericCommand([=] {player.accelerate({ 0,-mSpeed }); }));
	commander.addHeld(sf::Keyboard::S, new GenericCommand([=] {player.accelerate({ 0,mSpeed }); }));
	commander.addHeld(sf::Keyboard::A, new GenericCommand([=] {player.accelerate({ -mSpeed,0 }); }));
	commander.addHeld(sf::Keyboard::D, new GenericCommand([=] {player.accelerate({ mSpeed,0 }); }));

	commander.addPressed(sf::Keyboard::LShift, new GenericCommand([=] {cam.shake(15.f, 0.75f); }));
	commander.addPressed(sf::Keyboard::LAlt, new GenericCommand([=] {GameState::incrementLevel(); }));

	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {GameState::setCurrentState(State::PAUSE); }));

	// Camera + light
	cam = Camera(midWin, winSize);
	cam.follow(&player, 0.95f);

	lighter.setTarget(dynamic_cast<sf::RenderTexture*>(window));
	lighter.create();

}

void BaseLevel::handleInput(float dt)
{

}

void BaseLevel::update(float dt)
{

}


void BaseLevel::render()
{
}

void BaseLevel::doorCheck()
{
	if (killCount >= enemyCount) {
		// change door light to green if enough enemies have been killed
		std::get<0>(doorLight) = sf::Vector2f{ door.getPosition() - sf::Vector2f(-23.f, 70.f) };
		std::get<1>(doorLight) = 100.f;
		std::get<2>(doorLight) = sf::Color::Green;
		lighter.setLight(doorLightI, doorLight);

		if (Collision::checkBoundingBox(&player, &door)) {
			GameState::incrementLevel();
		}
	}

}

void BaseLevel::loadLevel(std::string const& filename)
{

	auto data = SceneDataLoader::loadScene(filename);
	sceneObjects = data.first;
	for (auto* obj : sceneObjects)
	{
		physMan.registerObj(obj, true);
	}
	for (auto const& light : data.second)
	{
		lighter.addLight(light);
	}

}

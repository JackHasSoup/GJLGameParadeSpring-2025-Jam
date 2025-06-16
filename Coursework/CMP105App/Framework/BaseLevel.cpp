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
	player.setDrawType(drawType::RECT);
	auto cs = sf::ConvexShape(4);
	cs.setPoint(0, { 0.f, 0.f });
	cs.setPoint(1, { player.getSize().x, 0.f });
	cs.setPoint(2, { player.getSize().x, player.getSize().y });
	cs.setPoint(3, { 0.f, player.getSize().y });
	player.setCollisionShape(cs);

	if (!heartShader.loadFromFile("shaders/heart.frag", sf::Shader::Type::Fragment))
	{
		std::cout << "Error loading healthbar shader";
	}
	heartShader.setUniform("texture", sf::Shader::CurrentTexture);

	healthBar = HealthBar(window, &player, &heartShader);

	physMan.registerObj(&player, false);

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

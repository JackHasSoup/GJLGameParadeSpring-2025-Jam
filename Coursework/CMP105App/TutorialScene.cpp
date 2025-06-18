#include "TutorialScene.h"

TutorialScene::TutorialScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	enemyCount = 0;
	killCount = 0;
	bgColor = sf::Color(75, 108, 145);

	floorTexture = AssetManager::registerNewTex("floor");
	floorTexture->loadFromFile("gfx/materials/floor.png");
	floorTexture->setRepeated(true);
	floor.setTexture(floorTexture);

	BaseLevel::loadLevel("levels/tutorial.json");

	doorTexture = AssetManager::registerNewTex("door");
	doorTexture->loadFromFile("gfx/materials/door.png");
	door.setTexture(doorTexture);

	door.setPosition(midWin - sf::Vector2f{ 1000.f,1780.f });

	player.positionReset(door.getPosition() + sf::Vector2f{0.f, (door.getSize().y)});

	commander.addPressed(sf::Keyboard::N, new GenericCommand([=] {player.damage(0.5f); }));
	commander.addPressed(sf::Keyboard::M, new GenericCommand([=] {player.restoreHealth(); }));
}

void TutorialScene::handleInput(float dt)
{
	commander.handleInput();

}

void TutorialScene::update(float dt)
{
	doorCheck(); // light changes colour if enough enemies killed, moves to next level if player collision and light is green

	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);

}

void TutorialScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	lighter.draw(&floor);

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&door);

	lighter.draw(&player);

	lighter.endDraw();

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

}

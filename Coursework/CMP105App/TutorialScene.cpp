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

	doorTexture = AssetManager::registerNewTex("door");
	doorTexture->loadFromFile("gfx/materials/door.png");
	door.setTexture(doorTexture);

	door.setPosition(midWin - sf::Vector2f{ 0,400 });

	lighter.addLight(midWin + sf::Vector2f(-125, 150), 600.f, sf::Color(193,240,244));

	//BaseLevel::loadLevel("levels/level.json");
	commander.addPressed(sf::Keyboard::Space, new GenericCommand([=] {GameState::setCurrentState(State::TEST); }));
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

}

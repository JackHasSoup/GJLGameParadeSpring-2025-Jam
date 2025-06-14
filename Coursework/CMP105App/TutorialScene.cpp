#include "TutorialScene.h"

TutorialScene::TutorialScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	enemyCount = 0;
	bgColor = sf::Color(75, 108, 145);

	lighter.addLight(midWin + sf::Vector2f(-125, 150), 250.f, sf::Color::Yellow);

	//BaseLevel::loadLevel("levels/level.json");
	commander.addPressed(sf::Keyboard::Space, new GenericCommand([=] {GameState::setCurrentState(State::TEST); }));
}

void TutorialScene::handleInput(float dt)
{
	commander.handleInput();

}

void TutorialScene::update(float dt)
{
	physMan.update(dt);

	cam.update(dt);


}

void TutorialScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&player);

	lighter.endDraw();

}

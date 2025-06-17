#include "TutorialScene.h"

TutorialScene::TutorialScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	enemyCount = 0;
	bgColor = sf::Color(75, 108, 145);

	//hitFlash = AssetManager::registerNewShader("flash");
	//if (!hitFlash->loadFromFile("shaders/hitFlash.frag", sf::Shader::Type::Fragment))
	//{
	//	std::cout << "Error loading healthbar shader";
	//}
	//hitFlash->setUniform("texture", sf::Shader::CurrentTexture);

	lighter.addLight(midWin + sf::Vector2f(-125, 150), 250.f, sf::Color::Yellow);

	//BaseLevel::loadLevel("levels/level.json");
	commander.addPressed(sf::Keyboard::Space, new GenericCommand([=] {GameState::setCurrentState(State::TEST); }));
	commander.addPressed(sf::Keyboard::N, new GenericCommand([=] {player.damage(0.5f); }));
	commander.addPressed(sf::Keyboard::M, new GenericCommand([=] {player.restoreHealth(); }));
}

void TutorialScene::handleInput(float dt)
{
	commander.handleInput();

}

void TutorialScene::update(float dt)
{
	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);

}

void TutorialScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	AssetManager::getShader("flash")->setUniform("timer", player.getHitTimer());
	lighter.draw(&player, player.getState());

	lighter.endDraw();

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

}

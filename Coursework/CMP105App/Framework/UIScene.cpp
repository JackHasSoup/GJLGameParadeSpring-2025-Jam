#include "UIScene.h"

UIScene::UIScene()
{
	window = nullptr;
}

UIScene::UIScene(sf::RenderTarget* hwnd)
{
	window = hwnd;
	font = AssetManager::getFont("scribble");

	bg.setSize(sf::Vector2f{ window->getSize() });
	bg.setTextureRect(sf::IntRect(0, 0, bg.getSize().x, bg.getSize().y));
}

void UIScene::handleInput(float dt)
{
	for (int i = 0; i < buttons.size(); i++) { 
		if (Input::isLeftMousePressed()) {
			buttons[i].checkInput(window);
		}
	}
}

void UIScene::update(float dt)
{
}

void UIScene::render()
{
	window->clear(sf::Color(38,60,82));

	window->draw(bg);

	renderButtons();

}

void UIScene::renderButtons()
{
	for (int i = 0; i < buttons.size(); i++) {
		window->draw(buttons[i]);
	}
}


void UIScene::changeState(State inputState)
{
	GameState::setCurrentState(inputState);
}

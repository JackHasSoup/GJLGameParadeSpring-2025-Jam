#include "UIScene.h"

UIScene::UIScene()
{
	font = AssetManager::getFont("arial");
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

	for (int i = 0; i < buttons.size(); i++) {
		window->draw(buttons[i]);
	}

}


void UIScene::changeState(State inputState)
{
	GameState::setCurrentState(inputState);
}

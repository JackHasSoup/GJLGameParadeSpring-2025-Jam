#include "IntroScene.h"

IntroScene::IntroScene(sf::RenderTarget* hwnd) : UIScene(hwnd)
{

	bgTexture = AssetManager::getTex("bgTile");
	bg.setTexture(bgTexture);

	buttons.resize(1);

	buttons[ButtonIndex::Play] = Button((sf::Vector2f(window->getSize().x, window->getSize().y) - (sf::Vector2f(winSize) * 0.15f)), winSize * 0.125f, 56, font, "Start", true);
	buttons[ButtonIndex::Play].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Play].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Play].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Play].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Play].subscribe([=] { GameState::setCurrentState(State::TUTORIAL); });

	storyText.setCharacterSize(48);
	storyText.setFont(*font);
	storyText.setString("As the last living creature in this facility, you must defeat the counterfeit creatures to escape!\nThey look like animals, and bleed motor oil, but as robots, all they can do is copy what they see.\nUsing your moves will cause the creatures to mimic them back at you.");
	storyText.setFillColor(sf::Color::White);
	storyText.setPosition(midWin - sf::Vector2f{ storyText.getLocalBounds().width/2.f, winSize.y * 0.4f});

	controlsText.setCharacterSize(48);
	controlsText.setFont(*font);
	controlsText.setString("CONTROLS\n\nWASD - Directional Movement\nLeft Mouse Click - Light Attack\nRight Mouse Click - Heavy Attack\nSpacebar - Dodge\nQ - Parry");
	controlsText.setFillColor(sf::Color::White);
	controlsText.setPosition(midWin - sf::Vector2f{ storyText.getLocalBounds().width / 2.f, winSize.y * 0.175f });

}

void IntroScene::handleInput(float dt)
{
	UIScene::handleInput(dt);

	commander.handleInput();

}

void IntroScene::render()
{
	UIScene::render();

	window->draw(storyText);
	window->draw(controlsText);
}

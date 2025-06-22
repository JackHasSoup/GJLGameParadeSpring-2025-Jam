#include "GameOverLoseScreen.h"

GameOverLoseScreen::GameOverLoseScreen(sf::RenderTarget* hwnd, sf::RenderWindow* screenHwnd) : UIScene(hwnd)
{
	screenWindow = screenHwnd;

	bgTexture = AssetManager::getTex("bgTile");
	bg.setTexture(bgTexture);

	titleText.setCharacterSize(72);
	titleText.setFont(*font);
	titleText.setString("You Died :(");
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(midWin - sf::Vector2f{ titleText.getLocalBounds().width / 2.f, winSize.y * 0.3f });

	buttons.resize(3);

	buttons[ButtonIndex::Play] = Button(midWin, winSize * 0.125f, 48, font, "Play Again", true);
	buttons[ButtonIndex::Play].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Play].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Play].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Play].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Play].subscribe([=] { GameState::setCurrentState(State::TUTORIAL); });

	buttons[ButtonIndex::Menu] = Button(midWin + sf::Vector2f{ 0,winSize.y * 0.15f }, winSize * 0.125f, 48, font, "Menu", true);
	buttons[ButtonIndex::Menu].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Menu].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Menu].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Menu].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Menu].subscribe([=] { GameState::setCurrentState(State::MENU); });

	buttons[ButtonIndex::Exit] = Button(midWin + sf::Vector2f{ 0,winSize.y * 0.3f }, winSize * 0.125f, 48, font, "Exit", true);
	buttons[ButtonIndex::Exit].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Exit].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Exit].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Exit].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Exit].subscribe([=] { screenWindow->close(); });

	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {screenWindow->close(); }));

}


void GameOverLoseScreen::handleInput(float dt)
{
	UIScene::handleInput(dt);

	commander.handleInput();
}

void GameOverLoseScreen::render()
{
	UIScene::render();

	window->draw(titleText);

}

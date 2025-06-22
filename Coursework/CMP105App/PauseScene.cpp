#include "PauseScene.h"

PauseScene::PauseScene(sf::RenderTarget* hwnd) : UIScene(hwnd)
{
	pausedState = State::PAUSE;

	overlay.setFillColor(sf::Color(6, 15, 26,170));
	overlay.setPosition(sf::Vector2f{ 0,0 });

	titleText.setCharacterSize(60);
	titleText.setFont(*font);
	titleText.setString("Paused");
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(midWin - sf::Vector2f{ titleText.getLocalBounds().width / 2.f, winSize.y * 0.3f });

	controlsText.setFont(*font);
	controlsText.setString("CONTROLS\n\nWASD - Directional Movement\nLeft Mouse Click - Light Attack\nRight Mouse Click - Heavy Attack\nSpacebar - Dodge\nQ - Parry");
	controlsText.setFillColor(sf::Color::White);
	controlsText.setPosition(sf::Vector2f{ winSize.x * 0.05f, winSize.y * 0.3f });
	controlsText.setCharacterSize(int(winSize.x / controlsText.getLocalBounds().width) * 8);

	buttons.resize(2);

	buttons[ButtonIndex::Resume] = Button(midWin, winSize * 0.125f, 48, font, "Resume", true);
	buttons[ButtonIndex::Resume].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Resume].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Resume].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Resume].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Resume].subscribe([=] { GameState::setCurrentState(pausedState); }); // Resume to the state you paused from

	buttons[ButtonIndex::MainMenu] = Button(midWin + sf::Vector2f{ 0,winSize.y * 0.2f }, winSize * 0.125f, 48, font, "Main Menu", true);
	buttons[ButtonIndex::MainMenu].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::MainMenu].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::MainMenu].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::MainMenu].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::MainMenu].subscribe([=] { GameState::setCurrentState(State::MENU); });

}

void PauseScene::beginDraw()
{
	window->clear(sf::Color::White);
}

void PauseScene::render()
{
	window->setView(window->getDefaultView()); // screen view

	overlay.setSize(sf::Vector2f{ window->getSize() });

	window->draw(overlay);

	UIScene::renderButtons();

	window->draw(titleText);

	window->draw(controlsText);
}

void PauseScene::setPausedState(State inputState)
{
	pausedState = inputState;
}

State PauseScene::getPausedState()
{
	return pausedState;
}

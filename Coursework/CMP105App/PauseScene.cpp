#include "PauseScene.h"

PauseScene::PauseScene(sf::RenderTarget* hwnd) : UIScene(hwnd)
{
	pausedState = State::PAUSE;

	overlay.setFillColor(sf::Color(26, 31, 48,150));
	overlay.setPosition(sf::Vector2f{ 0,0 });

	buttons.resize(2);

	buttons[ButtonIndex::Resume] = Button(midWin, winSize * 0.2f, 36, font, "Resume", true);
	buttons[ButtonIndex::Resume].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Resume].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Resume].subscribe([=] { GameState::setCurrentState(pausedState); }); // Resume to the state you paused from

	buttons[ButtonIndex::MainMenu] = Button(midWin + sf::Vector2f{ 0,winSize.y * 0.3f }, winSize * 0.2f, 36, font, "Main Menu", true);
	buttons[ButtonIndex::MainMenu].body().setFillColor(sf::Color::Black);
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
}

void PauseScene::setPausedState(State inputState)
{
	pausedState = inputState;
}

State PauseScene::getPausedState()
{
	return pausedState;
}

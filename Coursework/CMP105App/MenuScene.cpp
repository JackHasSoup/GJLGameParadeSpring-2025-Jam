#include "MenuScene.h"

MenuScene::MenuScene(sf::RenderTarget* hwnd, sf::RenderWindow* screenHwnd) : UIScene(hwnd)
{
	screenWindow = screenHwnd;

	titleText.setCharacterSize(56);
	titleText.setFont(*font);
	titleText.setString("Seal Evil AI Game Title Pending");
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(midWin - sf::Vector2f{titleText.getLocalBounds().width/2.f, winSize.y * 0.3f});

	buttons.resize(2);

	buttons[ButtonIndex::Play] = Button(midWin, winSize * 0.2f, 36, font, "Start", true);
	buttons[ButtonIndex::Play].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Play].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Play].subscribe([=] { GameState::setCurrentState(State::TUTORIAL); });

	buttons[ButtonIndex::Exit] = Button(midWin + sf::Vector2f{0,winSize.y * 0.3f}, winSize * 0.2f, 36, font, "Exit", true);
	buttons[ButtonIndex::Exit].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Exit].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Exit].subscribe( [=] { screenWindow->close(); });

	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {screenWindow->close(); }));

}


void MenuScene::handleInput(float dt)
{
	UIScene::handleInput(dt);

	commander.handleInput();
}

void MenuScene::render()
{
	UIScene::render();

	window->draw(titleText);

}

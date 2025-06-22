#include "MenuScene.h"

MenuScene::MenuScene(sf::RenderTarget* hwnd, sf::RenderWindow* screenHwnd) : UIScene(hwnd)
{
	screenWindow = screenHwnd;

	bgTexture = AssetManager::registerNewTex("bgTile");
	bgTexture->loadFromFile("gfx/ui/bgtile.png");
	bgTexture->setRepeated(true);

	titleTexture = AssetManager::registerNewTex("title");
	titleTexture->loadFromFile("gfx/ui/title.png");
	title.setTexture(titleTexture);
	title.setSize(winSize * 0.35f);
	title.setPosition(midWin - sf::Vector2f{ title.getLocalBounds().width / 2.f, winSize.y * 0.45f });

	bg.setTexture(bgTexture);

	buttons.resize(2);

	buttons[ButtonIndex::Play] = Button(midWin, winSize * 0.125f, 48, font, "Start", true);
	buttons[ButtonIndex::Play].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Play].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Play].body().setOutlineThickness(5.f);
	buttons[ButtonIndex::Play].msg().setFillColor(sf::Color::White);
	buttons[ButtonIndex::Play].subscribe([=] { GameState::setCurrentState(State::INTRO); });

	buttons[ButtonIndex::Exit] = Button(midWin + sf::Vector2f{0,winSize.y * 0.2f}, winSize * 0.125f, 48, font, "Exit", true);
	buttons[ButtonIndex::Exit].body().setFillColor(sf::Color::Black);
	buttons[ButtonIndex::Exit].body().setOutlineColor(sf::Color::White);
	buttons[ButtonIndex::Exit].body().setOutlineThickness(5.f);
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

	window->draw(title);

}

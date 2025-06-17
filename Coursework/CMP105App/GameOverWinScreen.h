#pragma once
#include "Framework/UIScene.h"

class GameOverWinScreen : public UIScene {
public:
	GameOverWinScreen(sf::RenderTarget* hwnd, sf::RenderWindow* screenHwnd);

	void handleInput(float dt);
	void render();

private:
	enum ButtonIndex { Play, Menu, Exit }; // enum for ease of referring to specific button indexes

	sf::Text titleText;

	sf::RenderWindow* screenWindow; // menu needs to have the direct window because a rendertarget can't be closed
};
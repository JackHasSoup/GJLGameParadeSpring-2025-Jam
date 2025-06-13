#pragma once
#include "Framework/UIScene.h"

class MenuScene : public UIScene {
public:
	MenuScene(sf::RenderTarget* hwnd, sf::RenderWindow* screenHwnd);

	void handleInput(float dt);
	void render();

private:
	enum ButtonIndex { Play, Exit };

	sf::Text titleText;

	sf::RenderWindow* screenWindow; // menu needs to have the direct window because a rendertarget can't be closed
};
#pragma once
#include "Framework/UIScene.h"

class IntroScene : public UIScene {
public:
	IntroScene(sf::RenderTarget* hwnd);

	void handleInput(float dt);
	void render();

private:
	enum ButtonIndex { Play }; // enum for ease of referring to specific button indexes


	sf::Text storyText;
	sf::Text controlsText;
};
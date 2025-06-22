#pragma once
#include "Scene.h"
#include "../GenericCommand.h"
#include "Button.h"

class UIScene : public Scene
{
public:
	UIScene();
	UIScene(sf::RenderTarget* hwnd);

	void handleInput(float dt);
	void update(float dt);
	void render();

	void renderButtons();

	void changeState(State inputState);

protected:
	sf::Font* font;
	std::vector<Button> buttons; // customisable vector of buttons, they are checked for input and rendered in the uiscreen base functions

	Commander commander;

	sf::Text titleText;

	sf::Texture* bgTexture;
	sf::RectangleShape bg;

};
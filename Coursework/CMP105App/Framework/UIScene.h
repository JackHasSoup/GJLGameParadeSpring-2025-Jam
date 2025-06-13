#pragma once
#include "Scene.h"
#include "../GenericCommand.h"
#include "Button.h"

class UIScene : public Scene
{
public:
	UIScene();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void changeState(State inputState);

protected:
	sf::Font* font;
	std::vector<Button> buttons;

	Commander commander;

};
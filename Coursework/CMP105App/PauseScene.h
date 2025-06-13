#pragma once
#include "Framework/UIScene.h"

class PauseScene : public UIScene {
public:
	PauseScene(sf::RenderTarget* hwnd);

	void beginDraw();
	void render();

	void setPausedState(State inputState);
	State getPausedState();

private:
	sf::RectangleShape overlay;

	enum ButtonIndex { Resume, MainMenu }; // enum for ease of referring to specific button indexes

	State pausedState; // The state the game was on before pausing

};
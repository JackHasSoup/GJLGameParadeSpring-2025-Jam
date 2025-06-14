#pragma once

#include <SFML/Graphics.hpp>
#include "VectorHelper.h"
#include "GameState.h"

class SceneTransition {
public:
	SceneTransition(sf::RenderTarget* hwnd);

	void fadeIn(float dt);
	void fadeOut(float dt);

	void render();

	void setTransition(State startState, State endState);

	State getStartState();
	State getEndState();
	
	float getTimerDefault();

	float getInTimer();
	float getOutTimer();


private:
	sf::RectangleShape overlay;

	State startState;
	State endState;

	float timerDefault;
	float inTimer;
	float outTimer;

	sf::RenderTarget* window;

};
#include "SceneTransition.h"
#include <iostream>

SceneTransition::SceneTransition(sf::RenderTarget* hwnd)
{
	window = hwnd;

	overlay.setSize(sf::Vector2f(window->getSize()));
	overlay.setFillColor(sf::Color(0,0,0,0));

	startState = State::NUMSCENES;
	endState = State::NUMSCENES;

	timerDefault = 0.4f;
	inTimer = 0.f;
	outTimer = 0.f;
}

void SceneTransition::fadeIn(float dt)
{
	inTimer -= dt;
	// start at 0% opacity fade to 100% opacity over half the timer
	overlay.setFillColor(sf::Color(overlay.getFillColor().r, overlay.getFillColor().g, overlay.getFillColor().b, std::clamp(floor(VectorHelper::lerp(255, 0, inTimer / timerDefault)),0.f,255.f)));
}

void SceneTransition::fadeOut(float dt)
{
	outTimer -= dt;
	// start at 100% opacity fade to 0% opacity over the second half of the timer
	overlay.setFillColor(sf::Color(overlay.getFillColor().r, overlay.getFillColor().g, overlay.getFillColor().b, std::clamp(floor(VectorHelper::lerp(0, 255, inTimer / timerDefault)), 0.f, 255.f)));
}

void SceneTransition::render()
{
	window->setView(window->getDefaultView());

	window->draw(overlay);
}

void SceneTransition::setTransition(State start, State end)
{
	startState = start;
	endState = end;

	inTimer = timerDefault;
	outTimer = timerDefault;
}

State SceneTransition::getStartState()
{
	return startState;
}

State SceneTransition::getEndState()
{
	return endState;
}

float SceneTransition::getTimerDefault()
{
	return timerDefault;
}

float SceneTransition::getInTimer()
{
	return inTimer;
}

float SceneTransition::getOutTimer()
{
	return outTimer;
}
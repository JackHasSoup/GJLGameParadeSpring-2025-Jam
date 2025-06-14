#include "GameState.h"
State GameState::currentState = State::MENU;
State GameState::lastState = State::MENU;
sf::RenderTarget* GameState::renderTarget = nullptr;

// Set the current state
void GameState::setCurrentState(State s)
{
	GameState::currentState = s;
}

// Returns the current state.
State GameState::getCurrentState()
{
	return GameState::currentState;
}

void GameState::setLastState(State s)
{
	GameState::lastState = s;
}

State GameState::getLastState()
{
	return GameState::lastState;
}

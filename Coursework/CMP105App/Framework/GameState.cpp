#include "GameState.h"
State GameState::currentState = State::MENU;

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
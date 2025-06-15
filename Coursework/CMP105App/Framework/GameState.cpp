#include "GameState.h"
#include <iostream>

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

void GameState::incrementLevel()
{
	if (GameState::currentState > State::LOSE && static_cast<int>(GameState::currentState) < (static_cast<int>(State::NUMSCENES) - 1)) { // If it is a level and not a UI screen

		GameState::currentState = static_cast<State>(static_cast<int>(GameState::currentState) + 1);

	}
}

#pragma once
#include "SFML\Graphics.hpp"
#ifndef MAX_ACTIONS
#define MAX_ACTIONS 8
#endif // !MAX_ACTIONS


// Possible states. This enum can be modified to add additional states as required.
enum class State : int { MENU, PAUSE, TRANSITION, WIN, LOSE, TUTORIAL, TEST, NUMSCENES}; // Everything after LOSE is a level, everything before it is UI. Levels are stored in the order they appear in-game

struct CollisionData {
	bool collision = false;
	sf::Vector2f contactPoint = {};
	sf::Vector2f normal = {};
	float penetrationDepth = 0.f;
};

class GameState
{
public:
	static void setCurrentState(State s);
	static State getCurrentState();

	static void setLastState(State s);
	static State getLastState();

	static void incrementLevel();

	static sf::RenderTarget* getRenderTarget() { return renderTarget; };
	static void setRenderTarget(sf::RenderTarget* rt) {renderTarget = rt;};
protected:
	static State currentState;
	static State lastState;
	static sf::RenderTarget* renderTarget;
};
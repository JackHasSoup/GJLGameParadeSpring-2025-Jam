#pragma once
#include "SFML\Graphics.hpp"
#ifndef MAX_ACTIONS
#define MAX_ACTIONS 8
#endif // !MAX_ACTIONS


// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, PAUSE, WIN, LOSE, LEVEL };

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

	static sf::RenderTarget* getRenderTarget() { return renderTarget; };
	static void setRenderTarget(sf::RenderTarget* rt) {renderTarget = rt;};
protected:
	static State currentState;
	static State lastState;
	static sf::RenderTarget* renderTarget;
};
#pragma once
#include "Framework/BaseLevel.h"
#include "GenericCommand.h"
#include "BufferedCommand.h"
#include "Player.h"

class JellyScene : public BaseLevel {
public:
	JellyScene(sf::RenderTarget* hwnd);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

	void reset() override;


private:

	std::vector<PhysicsObject*> spotlights;
	std::vector<PhysicsObject*> tubes;
};
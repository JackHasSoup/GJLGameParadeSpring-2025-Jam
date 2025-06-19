#pragma once
#include "Framework/BaseLevel.h"
#include "GenericCommand.h"
#include "BufferedCommand.h"
#include "Player.h"

class LevelJack : public BaseLevel
{
public:
	LevelJack(sf::RenderTarget* hwnd);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
};


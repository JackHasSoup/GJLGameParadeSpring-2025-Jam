#pragma once
#pragma once
#include "Framework/BaseLevel.h"
#include "GenericCommand.h"
#include "BufferedCommand.h"
#include "Player.h"

class BossScene : public BaseLevel {
public:
	BossScene(sf::RenderTarget* hwnd);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

	void reset() override;

private:

};
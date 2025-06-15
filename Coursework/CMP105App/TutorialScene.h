#pragma once
#include "Framework/BaseLevel.h"
#include "GenericCommand.h"
#include "BufferedCommand.h"
#include "Player.h"

class TutorialScene : public BaseLevel {
public:
	TutorialScene(sf::RenderTarget* hwnd);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:

};
#pragma once
#include "Framework/BaseLevel.h"
#include "Narwhal.h"

class RoomTestScene : public BaseLevel
{
public:
	RoomTestScene(sf::RenderTarget* hwnd);

	virtual void handleInput(float dt) override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
};


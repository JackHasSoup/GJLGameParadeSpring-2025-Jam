#pragma once
#include "Framework/Scene.h"
#include "Framework/Button.h"
#include "Framework/Collision.h"
#include "Framework/PhysicsObject.h"
#include "Framework/PhysicsManager.h"
#include "StackedObject.h"
#include "GenericCommand.h"
#include "Framework/Camera.h"
#include "Framework/DeferredIllumination.h"
#include "EDITOR/SceneDataLoader.h"
#include "Player.h"


class TestScene : public Scene
{
public:
	TestScene(sf::RenderTarget* hwnd);
	~TestScene() {};

	virtual void update(float dt) override;
	virtual void handleInput(float dt) override;
	virtual void render() override;

	void changeText(const sf::String& msg);
protected:
	Button button;
	sf::Font* font;
	Camera cam;

	Commander commander;
	Command* updateText;

	StackedObject stackSprite;

	std::vector<PhysicsObject*> sceneObjects;
	PhysicsManager physMan;
	PhysicsObject g1, g2, rock;
	sf::ConvexShape c1, c2, cR;

	DeferredIllumination lighter;
	int movingLightI;
	Light movingLight;

	sf::Texture* arrow;

	float r=0;
	float mSpeed = 350.f;
	float hue = 0.f;
	Player player;
};


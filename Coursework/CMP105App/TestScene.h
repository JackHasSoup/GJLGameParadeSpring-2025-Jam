#pragma once
#include "Framework/Scene.h"
#include "StackedObject.h"
#include "GenericCommand.h"
#include "BufferedCommand.h"
#include "EDITOR/SceneDataLoader.h"
#include "Player.h"
#include "BaseEnemy.h"
#include "Crab.h"
#include "Narwhal.h" 


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
	Crab crab;
	Narwhal nar;

	std::vector<CreatureObject*> enemies;

	//action buffer
	std::vector<BufferedCommand*> availableActions;
	std::vector<BufferedCommand*> actionBuffer;
	int oldestAction = 0;
	int maxActBufferSize = 8;

	void executeAndTrack(BufferedCommand* b);
};


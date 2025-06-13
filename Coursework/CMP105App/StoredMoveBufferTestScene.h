#pragma once
#include "Framework/Scene.h"
#include "BufferedCommand.h"

class StoredMoveBufferTestScene : public Scene
{
public:
	StoredMoveBufferTestScene(sf::RenderTarget* hwnd);
	~StoredMoveBufferTestScene() {};

	virtual void update(float dt) override;
	virtual void handleInput(float dt) override;
	virtual void render() override;
private:
	Commander cmndr;
	Camera cam;

	PhysicsObject player, enemyStandin;

	std::vector<BufferedCommand*> actionList;
	int oldestAction;
	int maxActions = 6; // only for example, can be more/less/determined by difficultt option
};


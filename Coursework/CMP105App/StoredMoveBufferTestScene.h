#pragma once
#include "Framework/Scene.h"
#include "BufferedCommand.h"
#include "GenericCommand.h"

class StoredMoveBufferTestScene : public Scene
{
public:
	StoredMoveBufferTestScene(sf::RenderTarget* hwnd);
	~StoredMoveBufferTestScene() {};

	virtual void update(float dt) override;
	virtual void handleInput(float dt) override;
	virtual void render() override;

	void executeAndTrack(BufferedCommand* b);
private:
	Commander cmndr;
	Camera cam;

	PhysicsObject player, enemyStandin;

	std::vector<BufferedCommand*> availableActions;
	std::vector<BufferedCommand*> actionList;
	int oldestAction = 0;
	int maxActions = 6; // only for example, can be more/less/determined by difficultt option
	int performingAction = 0;

	float cooldown, maxCooldown = 1.75f;
};


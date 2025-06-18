#pragma once
#include "Scene.h"
#include "../HealthBar.h"
#include "../Player.h"
#include "../GenericCommand.h"
#include "../EDITOR/SceneDataLoader.h"
#include "../BufferedCommand.h"
#include "../Room.h" 

class BaseLevel : public Scene {
public:
	BaseLevel();
	BaseLevel(sf::RenderTarget* hwnd);

	virtual void handleInput(float dt) override = 0;
	virtual void update(float dt) override = 0;
	virtual void render() override = 0;

	void loadLevel(std::string const& filename);

protected:
	Camera cam;

	Commander commander;

	std::vector<PhysicsObject*> sceneObjects;
	PhysicsManager physMan;

	HealthBar healthBar;
	sf::Shader* hitFlashShader;

	PhysicsObject door;

	sf::Color bgColor;
	DeferredIllumination lighter;

	Player player;


	int enemyCount; // how many enemies the player must kill to progress

	// action buffer
	std::vector<BufferedCommand*> availableActions;
	std::vector<BufferedCommand*> actionBuffer;
	int oldestAction = 0;
	int maxActBufferSize = 8;

	void executeAndTrack(BufferedCommand* b);

	//rooms
	std::vector<Room> rooms; // all rooms in the level
	Room* activeRoom = nullptr;
};
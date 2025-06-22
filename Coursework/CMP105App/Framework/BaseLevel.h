#pragma once
#include "Scene.h"
#include "../HealthBar.h"
#include "../Player.h"
#include "../GenericCommand.h"
#include "../EDITOR/SceneDataLoader.h"
#include "../BufferedCommand.h"
#include "../Room.h" 
#include "../Crab.h"
#include "../Narwhal.h" 
#include "../Jellyfish.h"
#include "../Walrus.h"

class BaseLevel : public Scene {
public:
	BaseLevel();
	BaseLevel(sf::RenderTarget* hwnd);

	virtual void handleInput(float dt) override = 0;
	virtual void update(float dt) override = 0;
	virtual void render() override = 0;

	virtual void reset();
	Player* getPlayer() { return &player; }

	void doorCheck();

	void loadLevel(std::string const& filename);

protected:
	Camera cam;

	Commander commander;

	std::vector<PhysicsObject*> sceneObjects;
	PhysicsManager physMan;

	HealthBar healthBar;
	sf::Shader* hitFlashShader;

	GameObject floor;
	std::vector<GameObject*> floors;

	std::vector<GameObject*> roomFog;
	sf::Texture* fogTexture;

	PhysicsObject door;
	PhysicsObject spotlight;
	PhysicsObject tube;

	sf::Texture* floorTexture;
	sf::Texture* doorTexture;
	sf::Texture* spotlightTexture;
	sf::Texture* tubeTexture;

	int doorLightI;
	Light doorLight;

	sf::Color bgColor;
	DeferredIllumination lighter;

	Player player;

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
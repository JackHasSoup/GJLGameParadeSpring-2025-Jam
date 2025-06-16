#pragma once
#include "Scene.h"
#include "../HealthBar.h"
#include "../Player.h"
#include "../GenericCommand.h"
#include "../EDITOR/SceneDataLoader.h"

class BaseLevel : public Scene {
public:
	BaseLevel();
	BaseLevel(sf::RenderTarget* hwnd);

	virtual void handleInput(float dt) override;
	virtual void update(float dt) override;
	virtual void render() override;

	void loadLevel(std::string const& filename);

protected:
	Camera cam;

	Commander commander;

	std::vector<PhysicsObject*> sceneObjects;
	PhysicsManager physMan;

	HealthBar healthBar;
	sf::Shader heartShader;

	PhysicsObject door;

	sf::Color bgColor;
	DeferredIllumination lighter;

	Player player;

	float mSpeed = 350.f;

	int enemyCount; // how many enemies the player must kill to progress


};
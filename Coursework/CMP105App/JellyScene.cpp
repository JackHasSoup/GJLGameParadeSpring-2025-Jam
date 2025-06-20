#include "JellyScene.h"

JellyScene::JellyScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{

	bgColor = sf::Color(95, 81, 133);

	floorTexture = AssetManager::getTex("floor");
	doorTexture = AssetManager::getTex("door");
	spotlightTexture = AssetManager::getTex("spotlight");
	tubeTexture = AssetManager::getTex("tube");

	BaseLevel::loadLevel("levels/jelly3.json");

	commander.addPressed(sf::Keyboard::N, new GenericCommand([=] {player.damage(0.5f); }));
	commander.addPressed(sf::Keyboard::M, new GenericCommand([=] {player.restoreHealth(); }));
}

void JellyScene::reset()
{
	BaseLevel::reset();
}


void JellyScene::handleInput(float dt)
{
	commander.handleInput();

	if (Input::isLeftMousePressed())
	{
		executeAndTrack(availableActions[0]);
	}
	if (Input::isRightMousePressed())
	{
		executeAndTrack(availableActions[1]);
	}
}

void JellyScene::update(float dt)
{
	for (auto& room : rooms)
	{
		if (room.checkForPlayer(&player, &room == activeRoom)) // check if player is in the room
		{
			activeRoom = &room; // set the active room to the one the player is in
		}
	}
	if (activeRoom) activeRoom->updateCreatures(actionBuffer, dt); // update creatures in the active room

	doorCheck(); // light changes colour if enough enemies killed, moves to next level if player collision and light is green

	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);
}

void JellyScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	lighter.draw(&floor);


	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&door);

	if (activeRoom) activeRoom->render(&lighter); // enemies inside room

	lighter.draw(&player);

	lighter.endDraw();

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

	window->setView(cam);
}

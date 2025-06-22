#include "BossScene.h"

BossScene::BossScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	bgColor = sf::Color(30, 30, 50);

	floorTexture = AssetManager::getTex("floor");
	fogTexture = AssetManager::getTex("fog");
	doorTexture = AssetManager::getTex("door");
	spotlightTexture = AssetManager::getTex("spotlight");
	tubeTexture = AssetManager::getTex("tube");

	BaseLevel::loadLevel("levels/boss.json");


}

void BossScene::reset()
{
	BaseLevel::reset();
}

void BossScene::handleInput(float dt)
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

void BossScene::update(float dt)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].checkForPlayer(&player, &rooms[i] == activeRoom)) // check if player is in the room
		{
			activeRoom = &rooms[i]; // set the active room to the one the player is in


			for (int j = 0; j < roomFog.size(); j++) {
				roomFog[j]->setFillColor(sf::Color(255, 255, 255, 200));
			}
			roomFog[i]->setFillColor(sf::Color::Transparent);
		}
	}
	if (activeRoom) activeRoom->updateCreatures(actionBuffer, dt); // update creatures in the active room

	doorCheck(); // light changes colour if enough enemies killed, moves to next level if player collision and light is green

	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);
}

void BossScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	for (auto& f : floors)
	{
		lighter.draw(f);
	}

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&door);

	if (activeRoom) activeRoom->render(&lighter); // enemies inside room

	lighter.draw(&player);

	lighter.endDraw();

	for (int i = 0; i < roomFog.size(); i++) {
		window->draw(*roomFog[i]);
	}

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

	window->setView(cam);
}


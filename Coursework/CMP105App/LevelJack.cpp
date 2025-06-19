#include "LevelJack.h"

LevelJack::LevelJack(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	floorTexture = AssetManager::getTex("floor");
	floor.setTexture(floorTexture);

	BaseLevel::loadLevel("levels/jack.json");

	//give crabs their directions
	dynamic_cast<Crab*>(rooms[0].getCreatures()[0])->setDirection({ 1.f,0.f });
	dynamic_cast<Crab*>(rooms[0].getCreatures()[1])->setDirection({ 0.f,1.f });
	dynamic_cast<Crab*>(rooms[0].getCreatures()[2])->setDirection(VectorHelper::normalise({ 1.f,-1.f }));
	dynamic_cast<Crab*>(rooms[3].getCreatures()[2])->setDirection(VectorHelper::normalise({ 1.f,1.f }));


	sceneObjects[0]->setDrawType(static_cast<drawType>(-1)); //NEVER RENDERERD

	door.setPosition(sceneObjects[0]->getPosition()); //use first placed object to position door
	door.setTexture(AssetManager::getTex("door"));
	doorLightI = lighter.addLight(door.getPosition() - sf::Vector2f(-23.f, 70.f), 100.f, sf::Color::Red);
	doorLight = Light(door.getPosition() - sf::Vector2f(0, 40.f), 50.f, sf::Color::Red);

	player.positionReset(door.getPosition() + sf::Vector2f{ 0.f, (door.getSize().y) });

	cam.setCenter(player.getPosition());
}

void LevelJack::handleInput(float dt)
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

void LevelJack::update(float dt)
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

void LevelJack::render()
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

	// Above lighting pass

	// HUD
	window->setView(window->getDefaultView()); //problem, update gets called after this function and the view is wrong for worldspace mouse calculations

	healthBar.render();

	window->setView(cam); //fix, set the camera back
}

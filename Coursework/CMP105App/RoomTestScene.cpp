#include "RoomTestScene.h"

RoomTestScene::RoomTestScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	rooms = {
		Room(0, 0, 800, 600, &player),
		Room(800, 0, 800, 600, &player),
		Room(0, 600, 800, 600, &player),
		Room(800, 600, 800, 600, &player)
	};

	//activeRoom = &rooms[0]; // set the first room as the active room
	//for each room add some narwhals randomly spread within that room
	for (auto& room : rooms)
	{
		for (int i = 0; i < 5; ++i)
		{
			auto* e = new Narwhal(
				sf::Vector2f(room.left + rand() % (int)room.width, room.top + rand() % (int)room.height),
				sf::Vector2f(200.f, 200.f), 150.f);
			e->setRotationLock(true);
			e->setAlive(true);
			e->setActive(true);
			room.addCreature(e);
			physMan.registerObj(e, false);
		}
	}
}

void RoomTestScene::handleInput(float dt)
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

void RoomTestScene::update(float dt)
{
	for (auto& room : rooms)
	{
		if (room.checkForPlayer(&player, &room == activeRoom)) // check if player is in the room
		{
			activeRoom = &room; // set the active room to the one the player is in
		}
	}
	if (activeRoom) activeRoom->updateCreatures(actionBuffer, dt); // update creatures in the active room

	physMan.update(dt);

	cam.update(dt);
}

void RoomTestScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);
	if (activeRoom) activeRoom->render(&lighter);
	lighter.draw(&player);
	lighter.endDraw();

	//debug draw rects for each room
	for (const auto& room : rooms)
	{
		sf::RectangleShape rect(sf::Vector2f(room.width, room.height));
		rect.setPosition(room.left, room.top);
		rect.setFillColor(sf::Color(0, 0, 0, 50)); // semi-transparent fill
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(1.f);
		window->draw(rect);
	}
}

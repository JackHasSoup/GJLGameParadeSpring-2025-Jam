#include "Room.h"

Room::Room(float left, float top, float width, float height, Player* player, Room* sceneActiveRoom) : sf::FloatRect(left, top, width, height)
{
	this->player = player;
	this->sceneActiveRoom = sceneActiveRoom;
}

void Room::checkForPlayer(Player* p)
{
	bool in = contains(p->getPosition());

	if (sceneActiveRoom == this)
	{
		if (!in)
		{
			onExit();
			//do NOT set nullptr here, as if going through all rooms in sequence then if a room is entered before this one realised it has been exited it might set the active room to nullptr, which would cause issues
			//*sceneActiveRoom = nullptr; // player has left the room
		}
	}
	else {
		if (in)
		{
			sceneActiveRoom = this; // player has entered the room
			onEnter();
		}
	}
}

void Room::render(DeferredIllumination* lighter)
{
	for (auto& e : enemies)
	{
		if (e->isAlive())
		{
			lighter->draw(e);
		}
	}
}

void Room::onEnter()
{
	for (auto& e : enemies)
	{
		e->setActive(true); // activate all creatures in this room
	}
}

void Room::onExit()
{
	for (auto& e : enemies)
	{
		e->setActive(false); // de-activate all creatures in this room
	}
}

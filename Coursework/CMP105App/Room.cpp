#include "Room.h"

Room::Room(float left, float top, float width, float height, Player* player) : sf::FloatRect(left, top, width, height)
{
	this->player = player;
}

bool Room::checkForPlayer(Player* p, bool amCurrent)
{
	bool in = contains(p->getPosition());

	if (!in && amCurrent)
	{
		onExit();
		//do NOT set nullptr here, as if going through all rooms in sequence then if a room is entered before this one realised it has been exited it might set the active room to nullptr, which would cause issues
		//*sceneActiveRoom = nullptr; // player has left the room
	}
	else {
		if (in)
		{
			onEnter();
			return true;
		}
	}
	return false;
}

void Room::updateCreatures(std::vector<BufferedCommand*> actionBuffer, float dt)
{
	for (auto& e : enemies)
	{
		if (e->isAlive() && dynamic_cast<BaseEnemy*>(e))
		{
			dynamic_cast<BaseEnemy*>(e)->trackPlayer(player, actionBuffer, dt);
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

bool Room::allCreaturesDead()
{
	if (allDead) return true; // if already known all are dead, return true immediately
	allDead = true; // assume all creatures are dead
	for (const auto& e : enemies)
	{
		if (e->isAlive())
		{
			allDead = false; // if any creature is alive, then not all are dead
			break;
		}
	}
	return allDead;
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

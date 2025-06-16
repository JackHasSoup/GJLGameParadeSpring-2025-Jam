#pragma once
#include <SFML/Graphics.hpp>
#include "BaseEnemy.h"
#include "Player.h"
#include "Framework/DeferredIllumination.h"

/*
* A room is a rectangular area defined by a FloatRect.
* It can be used to define boundaries for gameplay, such as
* the playable area in a game scene or to manage spatial partitioning.
* This class inherits from sf::FloatRect to utilize its properties

* Each room holds its own list of enemies and renders those enemies.
* It is responsible for ensuring creatures are only updated when the player is in the same room
* A room can detect when the player enters or leaves, for disabling or enabling enemies
*/
class Room : public sf::FloatRect
{
public:
	Room(float left, float top, float width, float height, Player* player);
	bool checkForPlayer(Player* p, bool amCurrent);

	std::vector<CreatureObject*> getCreatures() {return enemies;};
	void setCreatures(std::vector<CreatureObject*> const& creatures) { enemies = creatures; };
	void addCreature(CreatureObject* creature) { enemies.push_back(creature); };

	void updateCreatures(std::vector<BufferedCommand*> actionBuffer, float dt);
	void render(DeferredIllumination* lighter);

	bool allCreaturesDead();
private:
	Player* player;
	Room* sceneActiveRoom = nullptr; // the room that is currently active in the scene
	std::vector<CreatureObject*> enemies; // enemies in this room (must be kept as creatures for action buffer)
	bool allDead = false;

	void onEnter();
	void onExit();
};


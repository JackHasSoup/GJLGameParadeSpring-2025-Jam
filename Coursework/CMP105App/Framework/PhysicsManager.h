#pragma once
#include "PhysicsObject.h"
#include "Collision.h"

class PhysicsManager {
public:
	PhysicsManager() {};
	~PhysicsManager();

	void registerObj(PhysicsObject* newObj, bool stat);//registers an existing object
	PhysicsObject* registerNew(bool stat, float mass = 1.f);//creates a new registered object and returns it

	void update(float dt);
	//void setEdgeCollision(bool edge) { edgeCollision = edge; } //set if edge collision is enabled or not
private:
	std::vector<PhysicsObject*> staticObjects; //for non moving level objects
	std::vector<PhysicsObject*> dynamicObjects;
	//bool edgeCollision = true; // if true, will check edge collisions, otherwise will only check SAT
};
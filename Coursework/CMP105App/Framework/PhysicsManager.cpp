#include "PhysicsManager.h"

PhysicsManager::~PhysicsManager()
{
	/*for (auto* o : staticObjects)
	{
		delete o;
	}
	for (auto* o : dynamicObjects)
	{
		delete o;
	}*/
}

void PhysicsManager::registerObj(PhysicsObject* newObj, bool stat)
{
	if (stat)
	{
		staticObjects.push_back(newObj);
	}
	else {
		dynamicObjects.push_back(newObj);
	}
}

PhysicsObject* PhysicsManager::registerNew(bool stat, float mass)
{
	PhysicsObject* o = new PhysicsObject({}, {},mass);
	if (stat)
	{
		staticObjects.push_back(o);
	}
	else {
		dynamicObjects.push_back(o);
	}
	return o;
}

void PhysicsManager::update(float dt)
{
	//move all dynamic objects
	for (auto* d : dynamicObjects)
	{
		if(d->isAlive())d->update(dt);
	}

	//first test every dynamic object against eachother
	for (int i = 0; i < dynamicObjects.size(); ++i)
	{
		for (int j = i + 1; j < dynamicObjects.size(); ++j)
		{
			if (!dynamicObjects[i]->isAlive() || !dynamicObjects[j]->isAlive()) continue; //skip if either object is not alive
			//dynamiuc collision uses SAT for more reliable collision detection
			auto data = Collision::checkSAT(dynamicObjects[i], dynamicObjects[j]);
			if (data.collision)
			{
				const float ratio = 1.f / (dynamicObjects[i]->getMass() + dynamicObjects[j]->getMass());

				dynamicObjects[i]->collisionResponse(data, -1.f, ratio * dynamicObjects[j]->getMass(), dt);
				dynamicObjects[j]->collisionResponse(data, 1.f, ratio * dynamicObjects[i]->getMass(), dt);
			}
		}
	}

	//second check all dynamic objects against all static objects
	for (auto* d : dynamicObjects)
	{
		for (auto* s : staticObjects)
		{
			//static collision uses edge collision for convex geometry support
			auto data = Collision::checkEdgeCollision(d, s);
			if (data.collision)
			{
				d->setPosition(d->lastPos);
				d->collisionResponse(data, -0.5f, 1.f, dt);
			}
		}
	}
}

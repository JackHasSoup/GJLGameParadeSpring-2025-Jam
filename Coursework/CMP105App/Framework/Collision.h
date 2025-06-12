#pragma once
#include "PhysicsObject.h"
#include "VectorHelper.h"
#include "GameState.h"
#include <iostream>

class Collision
{
public:
	//check AABB for collision. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* sp1, GameObject* sp2);
	//check collision with box and point. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* s1, sf::Vector2i s2);
	//check bounding circle collision. Returns true if collision occurs.
	static bool checkBoundingCircle(GameObject* sp1, GameObject* sp2);
	//check really just the distance
	static bool checkSimpleCircle(GameObject* sp1, GameObject* sp2);

	//SAT collision
	static bool checkSATBool(GameObject* g1, GameObject* g2);
	static CollisionData checkSAT(GameObject* g1, GameObject* g2);

	static std::vector<sf::Vector2f> getCorners(GameObject& obj);
	static void projectPointsToAxis(const std::vector<sf::Vector2f>& corners, const sf::Vector2f& axis, float& min, float& max);

	static sf::Vector2f getCentre(GameObject& obj);

	//edge based collision
	static CollisionData checkEdgeCollision(GameObject* g1, GameObject* g2);
};
#pragma once
#include "KinematicObject.h"

class PhysicsObject : public KinematicObject
{
public:
	PhysicsObject(sf::Vector2f pos, sf::Vector2f size, float mass);
	PhysicsObject();//default constructor
	~PhysicsObject();

	virtual void collisionResponse(const CollisionData& data, float normCoef, const float& massRatio, float dt);
protected:
	static constexpr float resititutionCoef = 0.00015f;
	static constexpr float maxAllowedPenetration = 1.f;
};
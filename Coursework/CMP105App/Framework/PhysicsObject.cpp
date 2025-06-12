#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(sf::Vector2f pos, sf::Vector2f size, float mass) : KinematicObject(pos, size)
{
	this->mass = mass;
}

PhysicsObject::PhysicsObject() : KinematicObject()
{
	
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::collisionResponse(const CollisionData& data, float normCoef, const float& massRatio, float dt)
{
	//if (!data.collision) return; //exit if there wasn't a collision

	//move away from collision
	const float depth = std::min(data.penetrationDepth, maxAllowedPenetration);
	move(
		data.normal * normCoef * (massRatio * 0.5f * depth)
	);

	//apply torque
	if (rotationLock) return;
	const sf::Vector2f force = ((data.normal * depth) / (dt * dt)) * mass; //approximate force

	const float torque = VectorHelper::cross(data.contactPoint - getCentre(), force);
	angularlyAccelerate((torque / getMomentOfInertia()));
}

#pragma once
#include "GameObject.h"
#include "VectorHelper.h"
#include <algorithm>

class KinematicObject : public GameObject
{
public:
	KinematicObject(sf::Vector2f pos, sf::Vector2f size);
	KinematicObject(); // default constructor
	~KinematicObject();

	void handleInput(float dt) override;
	void update(float dt) override;

	// Velocity and Acceleration are using the Verlet integration
	// REFERENCE: Parts of this system have been referenced from the physics system I wrote in SFML for the Global Game Jam - Jack Sneddon (2309340)
	// Make sure when calling set/add velocity to pass deltatime otherwise you call the base gameobject functions and those do it different
	void setVelocity(sf::Vector2f const &vel, float const &dt);
	void addVelocity(sf::Vector2f const &vel, float const &dt);

	void setAngularVelocity(float const &a, float const &dt);
	void addAngularVelocity(float const &a, float const &dt);

	void positionReset(sf::Vector2f newPos);
	sf::Vector2f getVelocity(float const &dt);
	float getAngularVelocity(float const &dt);

	void accelerate(sf::Vector2f direction, float const &magnitude);
	void accelerate(sf::Vector2f const &acc);
	void angularlyAccelerate(float const &acc);
	float getDampCoef(float dt);

	sf::Vector2f getAcceleration() { return acceleration; };
	float getAngularAcceleration() { return angularAcceleration; };
	void resolveCollision(GameObject *other);

	void setRotationLock(bool l) { rotationLock = l; };
	bool getRotationLock() { return rotationLock; };

	void setMass(const float &newMass) { mass = newMass; };
	float getMass() { return mass; };

	void addForce(sf::Vector2f const &f) { acceleration += f / mass; };
	// a force vector applied from a position (in world coords)
	void addForceAtPoint(sf::Vector2f const &f, sf::Vector2f const &point);

	float getMomentOfInertia();

	sf::Vector2f getCentre();
	void calculateCentre();

	void setCanMove(bool c) { canMove = c; };
	bool getCanMove() { return canMove; };

protected:
	bool canMove = true;
	float mass = 1.f;
	float momentOfInertia = 0.f;

	bool rotationLock = true; // by default don't rotate
	float lastRotation = 0.f;
	float rotation = 0.f; // for integration

	sf::Vector2f acceleration;
	float angularAcceleration = 0.f;

	// larger dampening -> smaller velocity;
	float linearDampening = 3.5f;
	sf::Vector2f centre;
};
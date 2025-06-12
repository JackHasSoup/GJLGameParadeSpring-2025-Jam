#include "KinematicObject.h"
#include "Collision.h"

KinematicObject::KinematicObject(sf::Vector2f pos, sf::Vector2f size)
{
	setPosition(pos);
	lastPos = pos;

	setSize(size);

	setFillColor(sf::Color::Black);
	setOrigin(size / 2.f);
	setPosition(pos);

	setCollisionBox({ {},{getSize()} });
	setCollider(true);

	calculateCentre();
}

KinematicObject::KinematicObject()
{
	lastPos = {};
	setSize({ 10,10 });
}

KinematicObject::~KinematicObject()
{
}

void KinematicObject::handleInput(float dt)
{
	//most physics objects shouldn't need input handling, if the one you are creating does then consider whether that should actually be a new inherited class
}

void KinematicObject::update(float dt)
{
	//Find the difference in position, get velocity V from the change in position plus the acceleration, then slowly over time reduce velocity by linear dampening
	const sf::Vector2f delta = getPosition() - lastPos;
	lastPos = getPosition();
	const sf::Vector2f v = delta + (acceleration * dt * dt);
	setPosition(getPosition() + (v * getDampCoef(dt)));

	//reset accelartion to zero
	acceleration = {};

	if (rotationLock) return; //exit if don't rotate

	//integrate angular velocity
	const float deltaA = rotation - lastRotation;//std::abs(rotation - lastRotation) < std::abs(lastRotation - rotation) ? rotation - lastRotation : lastRotation - rotation;
	lastRotation = rotation;
	const float vA = deltaA + (angularAcceleration * dt * dt);
	rotation += vA * getDampCoef(dt);
	setRotation(rotation);

	angularAcceleration = {};
}

void KinematicObject::setVelocity(sf::Vector2f const& vel, float const& dt)
{
	//because in update velocity is calculated by the displacement of the object, fudge the previous position to create the desired velocity
	lastPos = getPosition() - vel * dt;
}

void KinematicObject::addVelocity(sf::Vector2f const& vel, float const& dt)
{
	//read comment in above function, same thing really
	lastPos -= vel * dt;
}

void KinematicObject::setAngularVelocity(float const& a, float const& dt)
{
	lastRotation = rotation - a * dt;
}

void KinematicObject::addAngularVelocity(float const& a, float const& dt)
{
	lastRotation -= a * dt;
}

void KinematicObject::positionReset(sf::Vector2f newPos)
{
	setVelocity({}, 1);
	setPosition(newPos);
	lastPos = getPosition();
}

sf::Vector2f KinematicObject::getVelocity(float const& dt)
{
	//V=s/t, velocity is displacement over time.
	return (getPosition() - lastPos) / dt;
}

float KinematicObject::getAngularVelocity(float const& dt)
{
	return (rotation - lastRotation) / dt;
}

void KinematicObject::accelerate(sf::Vector2f direction, float const& magnitude)
{
	direction = VectorHelper::normalise(direction);
	direction *= magnitude;
	acceleration += direction;
}

void KinematicObject::accelerate(sf::Vector2f const& acc)
{
	acceleration += acc;
}

void KinematicObject::angularlyAccelerate(float const& acc)
{
	angularAcceleration += acc;
}

float KinematicObject::getDampCoef(float dt)
{	//Reference: https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Rigidbody2D-linearDamping.html

	//return 1.f / ((1.f + dt) / linearDampening);
	return 1.f / (1.f + (dt * linearDampening));
}

void KinematicObject::resolveCollision(GameObject* other)
{
	//change behaviour in inherited objects
}

void KinematicObject::addForceAtPoint(sf::Vector2f const& f, sf::Vector2f const& point)
{
	addForce(f);
	//no need to make a rotationLock check, can angularly accelerate all I want the lock is checked in update

	const float torque = VectorHelper::cross(point - getCentre(), f);
	angularlyAccelerate((torque / getMomentOfInertia()));
}

float KinematicObject::getMomentOfInertia()
{
	if (momentOfInertia == 0.f) //only ever calculate this once
	{
		momentOfInertia = (1.f / 12.f) * mass * VectorHelper::magnitudeSqrd(getSize());
	}
	return momentOfInertia;
}

sf::Vector2f KinematicObject::getCentre()
{
	calculateCentre(); //if the object has moved then its centre must be recalculated
	return centre;
}

void KinematicObject::calculateCentre()
{
	int x = 0;
	int y = 0;

	float count = 0;
	for (auto& c : Collision::getCorners(*this))
	{
		count++;
		x += c.x;
		y += c.y;
	}
	centre = (sf::Vector2f(x, y) / count);
}

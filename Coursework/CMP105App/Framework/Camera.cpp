#include "Camera.h"

void Camera::update(float dt)
{
	//follow
	if (following)//nullptr check
	{
		if (followStrength*dt >= 1.f) { setCenter(following->getPosition()); }//avoid doing math when unnecesary
		else {
			setCenter(
				VectorHelper::lerp(getCenter(), following->getPosition() + panOffset + shakeOffset, followStrength*dt)
			);
		}
	}
	else {
		setCenter(
			getCenter() + (panOffset + shakeOffset)*dt
		);
	}
	panOffset = {}; //reset it for when the player stops panning


	//shake
	if (shakeLength > 0)
	{
		shakeOffset = sf::Vector2f(sin(shakeLength * 35.f) * 35.f, cos(shakeLength * 55.f) * 24.f) * shakeStrength;
		shakeLength -= dt * shakeDecreaseFactor;
	}
	else { shakeLength = 0.f; shakeOffset = {}; }
}

void Camera::shake(float strength, float length)
{
	shakeStrength = strength;
	shakeLength = length;
}

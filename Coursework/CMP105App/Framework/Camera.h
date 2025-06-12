#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "VectorHelper.h"

class Camera : public sf::View
{
public:
	Camera() {};
	Camera(const sf::Vector2f& center, const sf::Vector2f& size) : View(center, size) {};

	virtual void update(float dt);

	virtual void shake(float strength, float length);
	virtual void pan(const sf::Vector2f& p) { panOffset = p; };
	virtual void follow(GameObject* f, float strength = 1.f) { following = f; followStrength = strength; };
protected:
	GameObject* following = nullptr;
	sf::Vector2f panOffset;
	sf::Vector2f shakeOffset;
	float followStrength = 1.f;

	static constexpr float shakeDecreaseFactor = 1.f;
	float shakeStrength = 0.f;
	float shakeLength = 0.f;
};
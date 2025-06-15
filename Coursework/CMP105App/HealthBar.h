#pragma once
#include "Framework/GameObject.h"
#include "Framework/AssetManager.h"


class HealthBar : public GameObject {
public:
	HealthBar();
	HealthBar(float inputMaxHealth);

	void render(sf::RenderTarget* window);

	void setHealth(float inputHealth);

private:
	float currentHealth;
	float maxHealth;

	std::vector<GameObject> hearts;
	sf::Texture* heartTexture;

};
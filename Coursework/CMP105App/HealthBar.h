#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/GameObject.h"
#include "Framework/AssetManager.h"
#include "Player.h"


class HealthBar : public GameObject {
public:
	HealthBar();
	HealthBar(sf::RenderTarget* hwnd, Player* inputPlayer);

	void update(float dt);
	void render();

private:
	float currentHealth;
	float maxHealth;

	//sf::Shader heartShader;
	//sf::RenderStates state;

	std::vector<GameObject> hearts;
	sf::Texture* heartTexture;

	Player* player;

	sf::RenderTarget* window;

};
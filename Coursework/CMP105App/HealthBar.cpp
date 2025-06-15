#include "HealthBar.h"
#include <iostream>

HealthBar::HealthBar()
{
	window = nullptr;
}

HealthBar::HealthBar(sf::RenderTarget* hwnd, Player* inputPlayer)
{
	window = hwnd;

	player = inputPlayer;
	maxHealth = player->getMaxHealth();
	currentHealth = 0.f;

	sf::Vector2f startPos = sf::Vector2f{ 50.f,50.f };
	sf::Vector2f size = sf::Vector2f{ 100,100.f };

	setDrawType(drawType::RECT);

	heartTexture = AssetManager::registerNewTex("heart");
	heartTexture->loadFromFile("gfx/Seal/heart.png");

	//if (!heartShader.loadFromFile("shaders/heart.frag", sf::Shader::Type::Fragment))
	//{
	//	std::cout << "Error loading healthbar shader";
	//}
	//heartShader.setUniform("texture", sf::Shader::CurrentTexture);

	int numHearts = static_cast<int>(nearbyint(maxHealth));
	std::cout << numHearts << std::endl;
	hearts.resize(numHearts);

	for (int i = 0; i < hearts.size(); i++) {
		hearts[i].setSize(size);
		hearts[i].setPosition(startPos + sf::Vector2f{i * hearts[i].getSize().x, 0.f});
		hearts[i].setTexture(heartTexture);
		hearts[i].setAlive(true);
	}

}

void HealthBar::update(float dt)
{
	if (player->getHealth() != currentHealth) {
		currentHealth = player->getHealth();

		for (int i = 0; i < hearts.size(); i++) {
			if ((i + 1) > currentHealth) {
				hearts[i].setFillColor(sf::Color::Black);

			}
			else {
				hearts[i].setFillColor(sf::Color::White);
			}
		}
	}
}

void HealthBar::render()
{
	for (int i = 0; i < hearts.size(); i++) {
		window->draw(hearts[i]);
	}
}

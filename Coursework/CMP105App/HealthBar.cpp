#include "HealthBar.h"

HealthBar::HealthBar()
{

}

HealthBar::HealthBar(float inputMaxHealth)
{
	maxHealth = inputMaxHealth;
	currentHealth = 0.f;

	sf::Vector2f startPos = sf::Vector2f{ 50.f,50.f };
	sf::Vector2f size = sf::Vector2f{ 100,100.f };

	heartTexture = AssetManager::registerNewTex("heart");
	heartTexture->loadFromFile("gfx/Seal/heart.png");

	hearts.resize(ceil(maxHealth));
	for (int i = 0; i < ceil(maxHealth); i++) {
		hearts[i].setSize(size);
		hearts[i].setPosition(startPos + sf::Vector2f{i * hearts[i].getSize().x, 0.f});
		hearts[i].setTexture(heartTexture);
		hearts[i].setAlive(true);
	}

}

void HealthBar::render(sf::RenderTarget* window)
{
	for (int i = 0; i < hearts.size(); i++) {
		window->draw(hearts[i]);
	}
}

void HealthBar::setHealth(float inputHealth)
{
	currentHealth = inputHealth;

	for (int i = 0; i < maxHealth; i++) {
		if (i + i < currentHealth) {
			// If the current heart is higher than the current health value
			hearts[i].setAlive(false);
			hearts[i].setFillColor(sf::Color::Black);
		}
		else {
			hearts[i].setAlive(true);
			hearts[i].setFillColor(sf::Color::White);
		}
	}

}

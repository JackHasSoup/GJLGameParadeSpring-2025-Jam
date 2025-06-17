#include "HealthBar.h"
#include <iostream>

HealthBar::HealthBar()
{
	window = nullptr;
	heartShader = nullptr;
}

HealthBar::HealthBar(sf::RenderTarget* hwnd, Player* inputPlayer)
{
	window = hwnd;

	player = inputPlayer;
	maxHealth = player->getMaxHealth(); // This can be changed later if we want the player to gain more max health
	currentHealth = player->getHealth();

	//hitEffectTimer = 0.f;
	//effectTimerMax = 0.125f;

	heartShader = AssetManager::getShader("heart");

	startPos = sf::Vector2f{ 50.f,50.f };
	sf::Vector2f size = sf::Vector2f{ 100,100.f };

	setDrawType(drawType::RECT);

	heartTexture = AssetManager::registerNewTex("heart");
	heartTexture->loadFromFile("gfx/Seal/heart.png");

	int numHearts = static_cast<int>(round(maxHealth));
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
		if (player->getHealth() < currentHealth) {
			// If the player has been damaged then reset the hit effect timer
			hitEffectTimer = effectTimerMax;
		}
		currentHealth = player->getHealth();
	}
	hitEffectTimer -= dt;
	heartShader->setUniform("hitTimer", (player->getHitTimer() - (player->getHitTimerMax() / 2.f)));
	heartShader->setUniform("currentHealth", currentHealth);

	if (hitEffectTimer > 0.f) {
		for (int i = 0; i < hearts.size(); i++) {
		// Heart shaking up and down effect
		hearts[i].setPosition(hearts[i].getPosition() - sf::Vector2f{ 0.f, (cos(hitEffectTimer * TAU * 400.f) * 2.f) });
		}
	}
	else if(hitEffectTimer < 0.f && hitEffectTimer > -effectTimerMax) {
		for (int i = 0; i < hearts.size(); i++) {
			hearts[i].setPosition(startPos + sf::Vector2f{ i * hearts[i].getSize().x, 0.f });
		}
	}
}

void HealthBar::render()
{
	for (int i = 0; i < hearts.size(); i++) {
		heartShader->setUniform("index", (i * 1.f));
		window->draw(hearts[i], heartShader);
	}
}

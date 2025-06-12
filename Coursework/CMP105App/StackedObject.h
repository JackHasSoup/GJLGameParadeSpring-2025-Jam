#pragma once
#include "Framework/GameObject.h"

class StackedObject : public GameObject
{
public: 
	StackedObject() {};
	//Loads in a stack of layers from a spritesheet
	StackedObject(const sf::String& texPath, const float& spread, sf::Vector2i frameSize);
	~StackedObject() {};

	virtual void handleInput(float dt) override;
	virtual void update(float dt) override;

	virtual void setSize(const sf::Vector2f& size);
	virtual void setPosition(const sf::Vector2f& pos);
	virtual void setRotation(const float& rot);
	virtual void setOrigin(const sf::Vector2f& origin);

	virtual void move(const sf::Vector2f& delta);
	virtual void rotate(const float& delta);
protected:
	float spread = 0.f;
	sf::Texture* texture;
	std::vector<sf::RectangleShape> layers;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


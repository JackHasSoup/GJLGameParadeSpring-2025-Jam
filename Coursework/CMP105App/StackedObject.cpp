#include "StackedObject.h"

StackedObject::StackedObject(const sf::String& texPath, const float& spread, sf::Vector2i frameSize)
{
	this->spread = spread;

	texture = new sf::Texture;
	if (!texture->loadFromFile(texPath)) { std::cerr << "Error loading " << texPath.toAnsiString()<< "\n"; }

	int stackCount = texture->getSize().y / frameSize.y;

	layers.resize(stackCount);
	for (int i = 0; i < stackCount; i++)
	{
		RectangleShape layer;
		layer.setTextureRect({
			0,
			static_cast<int>(texture->getSize().y - (i*frameSize.y)),
			frameSize.x,
			frameSize.y
			});
		layer.setTexture(texture);

		layers[i] = layer;
	}
}

void StackedObject::handleInput(float dt)
{
}

void StackedObject::update(float dt)
{
}

void StackedObject::setSize(const sf::Vector2f& size)
{
	for (auto& layer : layers)
	{
		layer.setSize(size);
	}
	GameObject::setSize(size);
}

void StackedObject::setPosition(const sf::Vector2f& pos)
{
	int i = 0;
	for (auto& layer : layers)
	{
		layer.setPosition(
			pos.x - (layer.getSize().x / 2.f),
			pos.y - (layer.getSize().y / 2.f) - (i*spread)
		);
		i++;
	}
	GameObject::setPosition(pos);
}

void StackedObject::setRotation(const float& rot)
{
	for (auto& layer : layers)
	{
		layer.setRotation(rot);
	}
	GameObject::setRotation(rot);
}

void StackedObject::setOrigin(const sf::Vector2f& origin)
{
	for (auto& layer : layers)
	{
		layer.setOrigin(origin);
	}
	GameObject::setOrigin(origin);
}

void StackedObject::move(const sf::Vector2f& delta)
{
	for (auto& layer : layers)
	{
		layer.move(delta);
	}
	GameObject::move(delta);
}

void StackedObject::rotate(const float& delta)
{
	for (auto& layer : layers)
	{
		layer.rotate(delta);
	}
	GameObject::rotate(delta);
}

void StackedObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(static_cast<RectangleShape>(*this), states);
	for (auto& l : layers)
	{
		target.draw(l, states);
	}
}

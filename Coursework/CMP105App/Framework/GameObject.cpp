#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{

}

void GameObject::handleInput(float dt)
{
}

void GameObject::update(float dt)
{
}

sf::FloatRect GameObject::getCollisionBox() {
	
	return sf::FloatRect(collisionBox.left + getPosition().x - getOrigin().x, collisionBox.top + getPosition().y - getOrigin().y, collisionBox.width, collisionBox.height); 
}

sf::ConvexShape GameObject::getCollisionShape()
{
	auto s = collisionShape;
	s.setOrigin(getOrigin());
	s.setPosition(getPosition());
	s.setRotation(getRotation());
	s.setScale(getScale());
	
	return s;
}

sf::ConvexShape* GameObject::getRAWCollisionShape()
{
	collisionShape.setOrigin(getOrigin());
	collisionShape.setPosition(getPosition());
	collisionShape.setRotation(getRotation());
	collisionShape.setScale(getScale());
	return &collisionShape;
}

void GameObject::collisionResponse(GameObject * collider)
{
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto s = collisionShape;
	s.setOrigin(getOrigin());
	s.setPosition(getPosition());
	s.setRotation(getRotation());
	s.setScale(getScale());

	switch (drawType)
	{
	case drawType::RECT:
		target.draw(static_cast<RectangleShape>(*this), states);
		break;
	case drawType::COLLISION:
		target.draw(s, states);
		break;
	case drawType::BOTH_RC:
		target.draw(static_cast<RectangleShape>(*this), states);
		target.draw(s, states);
		break;
	case drawType::BOTH_CR:
		target.draw(s, states);
		target.draw(static_cast<RectangleShape>(*this), states);
		break;
	default:
		//if not set, assume intentional to set object invisible
		break;
	}
}

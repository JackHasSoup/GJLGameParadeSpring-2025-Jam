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
	case drawType::RECT_COL_LIGHTMASK:
		target.draw(static_cast<RectangleShape>(*this), states);
		break;
	case drawType::COL_RECT_LIGHTMASK:
		target.draw(s, states);
		break;
	default:
		//if not set, assume intentional to set object invisible
		break;
	}
}

void GameObject::makeSquareCollisionShape()
{
	collisionShape = sf::ConvexShape(4);
	collisionShape.setPointCount(4);
	collisionShape.setPoint(0, { 0.f, 0.f });
	collisionShape.setPoint(1, { getSize().x, 0.f });
	collisionShape.setPoint(2, { getSize().x, getSize().y });
	collisionShape.setPoint(3, { 0.f, getSize().y });
	baseHull = collisionShape;
}

void GameObject::makeCentredSquareCollisionShape()
{
	collisionShape = sf::ConvexShape(4);
	collisionShape.setPoint(0, sf::Vector2f(-getSize().x / 2.f, -getSize().y / 2.f));
	collisionShape.setPoint(1, sf::Vector2f(getSize().x / 2.f, -getSize().y / 2.f));
	collisionShape.setPoint(2, sf::Vector2f(getSize().x / 2.f, getSize().y / 2.f));
	collisionShape.setPoint(3, sf::Vector2f(-getSize().x / 2.f, getSize().y / 2.f));
	baseHull = collisionShape;
}

#pragma once
#include "SFML\Graphics.hpp"
#include "Input.h"
#include "GameState.h"
#include <iostream>

enum class drawType{ RECT, COLLISION, BOTH_RC, BOTH_CR, RECT_COL_LIGHTMASK, COL_RECT_LIGHTMASK};//last 2 use first name for rendering, 2nd for lighting shadows

class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	~GameObject();

	virtual void handleInput(float dt);
	virtual void update(float dt);

	bool isAlive() { return alive; };
	void setAlive(bool b) { alive = b; };

	bool isCollider() { return collider; };
	void setCollider(bool b) { collider = b; };
	sf::FloatRect getCollisionBox();
	sf::ConvexShape getCollisionShape();
	sf::ConvexShape* getRAWCollisionShape();
	sf::ConvexShape getBaseHull() { return baseHull; };
	void setCollisionBox(float x, float y, float width, float height) { collisionBox = sf::FloatRect(x, y, width, height); };
	void setCollisionBox(sf::FloatRect fr) { collisionBox = fr; };
	void setCollisionShape(sf::ConvexShape s) { collisionShape = s; baseHull = s; };
	virtual void collisionResponse(GameObject* collider);

	void setWindow(sf::RenderWindow* win) { window = win; };

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	drawType getDrawType() const { return drawType; }
	void setDrawType(drawType dt) { drawType = dt; }

	void makeCentredSquareCollisionShape();
	void makeSquareCollisionShape();

	virtual void setActive(bool a) { active = a; };
	virtual bool isActive() { return active; };

	sf::Vector2f lastPos;
protected:
	//sprite properties
	sf::Vector2f velocity;
	bool alive;
	bool active = true; //like alive but strictly for physics/lighting/loading

	//collision vars
	sf::FloatRect collisionBox;
	sf::ConvexShape collisionShape, baseHull; //for complex collision
	bool collider;

	sf::RenderWindow* window;
	drawType drawType = drawType::RECT;
};
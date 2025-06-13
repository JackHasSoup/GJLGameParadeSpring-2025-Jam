#pragma once

#include <SFML/Graphics.hpp>
#include "Input.h"
#include "AudioManager.h"
#include "GameState.h"
#include "Commander.h"
#include "AssetManager.h"
#include "Camera.h"
#include "DeferredIllumination.h"
#include "Button.h"
#include "Collision.h"
#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include <string>
#include <iostream>
#define winSize sf::Vector2f(window->getSize().x, window->getSize().y)
#define midWin (winSize/2.f)

class Scene
{
public:
	Scene();
	Scene(sf::RenderTarget* hwnd);

	~Scene();

	virtual void handleInput(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void render() = 0;

protected:
	//default functions for rendering to the screen.
	void beginDraw();

	//default variables for level class.
	sf::RenderTarget* window;
};


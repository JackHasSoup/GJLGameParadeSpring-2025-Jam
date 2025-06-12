#include "Scene.h"

Scene::Scene()
{
	window = nullptr;
}

Scene::Scene(sf::RenderTarget* hwnd)
{
	window = hwnd;
}

Scene::~Scene()
{
}

void Scene::beginDraw()
{
	window->clear(sf::Color(130, 112, 148));
}

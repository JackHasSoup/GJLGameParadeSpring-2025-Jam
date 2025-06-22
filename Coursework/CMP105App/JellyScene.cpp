#include "JellyScene.h"

JellyScene::JellyScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{

	bgColor = sf::Color(95, 81, 133);

	floorTexture = AssetManager::getTex("floor");
	doorTexture = AssetManager::getTex("door");
	spotlightTexture = AssetManager::getTex("spotlight");
	tubeTexture = AssetManager::getTex("tube");

	BaseLevel::loadLevel("levels/jelly3.json");

	for (int i = 0; i < lighter.getLightPos().size(); i++) {
		if (i == doorLightI) { break; }
		PhysicsObject* newLight = new PhysicsObject(sf::Vector2f{lighter.getLightPos().at(i)}, sf::Vector2f{75.f,75.f}, 10.f);
		sf::CircleShape c = sf::CircleShape(spotlight.getSize().x * 0.3f);
		sf::ConvexShape lightShape = sf::ConvexShape(c.getPointCount());
		for (int i = 0; i < c.getPointCount(); i++)
		{
			lightShape.setPoint(i, c.getPoint(i));
		}
		newLight->setCollisionShape(lightShape);
		newLight->setFillColor(sf::Color::White);
		newLight->setTexture(spotlightTexture);
		newLight->setDrawType(drawType::RECT);
		spotlights.push_back(newLight);
		physMan.registerObj(spotlights.back(), true);
	}

	commander.addPressed(sf::Keyboard::N, new GenericCommand([=] {player.damage(0.5f); }));
	commander.addPressed(sf::Keyboard::M, new GenericCommand([=] {player.restoreHealth(); }));
}

void JellyScene::reset()
{
	BaseLevel::reset();
}


void JellyScene::handleInput(float dt)
{
	commander.handleInput();

	if (Input::isLeftMousePressed())
	{
		executeAndTrack(availableActions[0]);
	}
	if (Input::isRightMousePressed())
	{
		executeAndTrack(availableActions[1]);
	}
}

void JellyScene::update(float dt)
{
	for (auto& room : rooms)
	{
		if (room.checkForPlayer(&player, &room == activeRoom)) // check if player is in the room
		{
			activeRoom = &room; // set the active room to the one the player is in
		}
	}
	if (activeRoom) activeRoom->updateCreatures(actionBuffer, dt); // update creatures in the active room

	doorCheck(); // light changes colour if enough enemies killed, moves to next level if player collision and light is green

	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);
}

void JellyScene::render()
{
	window->setView(cam);

	//lighter.beginDraw(bgColor);

	window->draw(floor);


	for (auto& o : sceneObjects)
	{
		window->draw(*o);
	}

	window->draw(door);

	//if (activeRoom) activeRoom->render(&lighter); // enemies inside room

	window->draw(player);

	//lighter.endDraw();

	for (auto& s : spotlights) {
		window->draw(*s);
	}

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

	window->setView(cam);
}

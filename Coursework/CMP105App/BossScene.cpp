#include "BossScene.h"

BossScene::BossScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	bgColor = sf::Color(0, 0, 0);

	floorTexture = AssetManager::getTex("floor");
	fogTexture = AssetManager::getTex("fog");
	doorTexture = AssetManager::getTex("door");
	spotlightTexture = AssetManager::getTex("spotlight");
	tubeTexture = AssetManager::getTex("tube");

	BaseLevel::loadLevel("levels/boss.json");

	// spotlight collision shape
	sf::CircleShape c = sf::CircleShape(spotlight.getSize().x * 0.33f, 9U);
	sf::ConvexShape lightShape = sf::ConvexShape(c.getPointCount());
	for (int i = 0; i < c.getPointCount(); i++)
	{
		lightShape.setPoint(i, c.getPoint(i));
	}

	for (int i = 0; i < lighter.getLightPos().size(); i++) {
		// Place a spotlight in the centre of every light except door light
		if (i == doorLightI) { break; }
		PhysicsObject* newLight = new PhysicsObject(sf::Vector2f{ lighter.getLightPos().at(i) }, sf::Vector2f{ 65.f,65.f }, 10.f);
		newLight->setCollisionShape(lightShape);
		newLight->setFillColor(sf::Color::White);
		newLight->setTexture(spotlightTexture);
		newLight->setDrawType(drawType::RECT);
		spotlights.push_back(newLight);
		physMan.registerObj(spotlights.back(), true);
	}

	// tube collision shape
	sf::ConvexShape ovalShape = sf::ConvexShape(c.getPointCount());
	for (int i = 0; i < ovalShape.getPointCount(); i++)
	{
		ovalShape.setPoint(i, sf::Vector2f{ lightShape.getPoint(i).x * (tube.getSize().x * 0.014f),lightShape.getPoint(i).y * (tube.getSize().y * 0.0055f) * 0.75f } + sf::Vector2f(0.f, tube.getSize().y * 0.72f));
	}

	tube = PhysicsObject(sf::Vector2f{ rooms[0].left + (rooms[0].width / 2.f), rooms[0].top + (rooms[0].height / 2.f) + 475.f },
		sf::Vector2f{ 175.f,437.5f },
		50.f);
	tube.setFillColor(sf::Color::White);
	tube.setTexture(tubeTexture);
	tube.setCollisionShape(ovalShape);
	tube.setDrawType(drawType::RECT_COL_LIGHTMASK);
	physMan.registerObj(&tube, true);

}

void BossScene::reset()
{
	BaseLevel::reset();
}

void BossScene::handleInput(float dt)
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

void BossScene::update(float dt)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].checkForPlayer(&player, &rooms[i] == activeRoom)) // check if player is in the room
		{
			activeRoom = &rooms[i]; // set the active room to the one the player is in
		}
	}
	if (activeRoom) activeRoom->updateCreatures(actionBuffer, dt); // update creatures in the active room

	doorCheck(); // light changes colour if enough enemies killed, moves to next level if player collision and light is green

	physMan.update(dt);

	cam.update(dt);

	healthBar.update(dt);
}

void BossScene::render()
{
	window->setView(cam);

	lighter.beginDraw(bgColor);

	for (auto& f : floors)
	{
		lighter.draw(f);
	}

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&door);

	if (activeRoom) activeRoom->render(&lighter); // enemies inside room

	lighter.draw(&player);

	lighter.draw(&tube);

	lighter.endDraw();

	for (auto& s : spotlights) {
		window->draw(*s);
	}

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

	window->setView(cam);
}


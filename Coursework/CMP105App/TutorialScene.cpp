#include "TutorialScene.h"

TutorialScene::TutorialScene(sf::RenderTarget* hwnd) : BaseLevel(hwnd)
{
	bgColor = sf::Color(0, 0, 0);

	// these textures only need to be loaded once ever

	font = AssetManager::registerNewFont("scribble");
	font->loadFromFile("font/friendlyscribbles.ttf");

	floorTexture = AssetManager::registerNewTex("floor");
	floorTexture->loadFromFile("gfx/materials/floor.png");

	fogTexture = AssetManager::registerNewTex("fog");
	fogTexture->loadFromFile("gfx/materials/roomfog.png");

	doorTexture = AssetManager::registerNewTex("door"); 
	doorTexture->loadFromFile("gfx/materials/door.png");

	spotlightTexture = AssetManager::registerNewTex("spotlight");
	spotlightTexture->loadFromFile("gfx/materials/light.png");

	tubeTexture = AssetManager::registerNewTex("tube");
	tubeTexture->loadFromFile("gfx/materials/tube.png");

	BaseLevel::loadLevel("levels/tutorial.json");

	// Manually placed objects

	for (int i = 0; i < 2; i++) {
		// Spawn two crabs in the room
		auto* e = new Crab(sf::Vector2f{ (rooms[0].left + rooms[0].width*( 0.5f + (0.33f * ((i * 2.f) - 1.f)))), (rooms[0].top + rooms[0].height * (0.55f + (0.02f * ((i * -2.f) + 1.f))))}, // convoluted way of making two crabs at opposite ends of the room
			{300.f, 150.f}, 
			20.f, 
			{ 1.f, (0.1f * ((i * 2.f) - 1.f))}); // slightly slant up or down
		e->setAlive(true);
		e->setActive(true);
		rooms[0].addCreature(e);
		physMan.registerObj(e, false);
	}

	spotlight.setPosition(lighter.getLightPos().at(0));
	physMan.registerObj(&spotlight, true);

	tube.setPosition(rooms[0].getCreatures().at(0)->getPosition() + sf::Vector2f{10.f,10.f});
	physMan.registerObj(&tube, true);

	// Debug

	commander.addPressed(sf::Keyboard::N, new GenericCommand([=] {player.damage(0.5f); }));
	commander.addPressed(sf::Keyboard::M, new GenericCommand([=] {player.restoreHealth(); }));
}

void TutorialScene::reset()
{
	BaseLevel::reset();

	//for (int i = 0; i < 2; i++) {
	//	// Reset position of enemies
	//	rooms[0].getCreatures().at(i)->positionReset(sf::Vector2f{ (rooms[0].left + rooms[0].width * (0.5f + (0.33f * ((i * 2.f) - 1.f)))), (rooms[0].top + rooms[0].height * (0.55f + (0.02f * ((i * -2.f) + 1.f)))) });
	//}

}


void TutorialScene::handleInput(float dt)
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

void TutorialScene::update(float dt)
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

void TutorialScene::render()
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

	// Above lighting pass

	window->draw(spotlight);

	// HUD
	window->setView(window->getDefaultView());

	healthBar.render();

	window->setView(cam);


}

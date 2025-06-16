#include "TestScene.h"
#define DEBUG_COL_POINTS

TestScene::TestScene(sf::RenderTarget* hwnd) : Scene(hwnd)
{
	font = AssetManager::registerNewFont("arial");
	font->loadFromFile("./font/arial.ttf");
	/*button = Button(midWin, winSize * 0.2f, 24, font, "Hello", true);
	button.body().setFillColor(sf::Color::Black);
	button.msg().setFillColor(sf::Color::Cyan);
	button.SUBSCRIBEA(TestScene, changeText, sf::String("Hello World"));*/

	c1 = sf::ConvexShape(3);
	c1.setOrigin(25, 25);
	c1.setPoint(0, { 0, 0 });
	c1.setPoint(1, { 50, 0 });
	c1.setPoint(2, { 50, 50 });

	c1.setFillColor(sf::Color::Cyan);
	c2 = c1;
	c1.setPointCount(4);
	c1.setPoint(3, { 0, 50 });

	g1 = PhysicsObject(midWin, { 50,50 }, 75);
	g1.setFillColor(sf::Color::Magenta);
	g1.setCollisionShape(c1);
	g1.setRotationLock(false);

	g2 = PhysicsObject(midWin / 1.2f, { 50,50 }, 15);
	g2.setFillColor(sf::Color::Green);
	g2.setCollisionShape(c2);
	g2.setRotationLock(false);

	//player setup
	player = Player(midWin, { 75.f, 75.f }, 20.f);

	crab = Crab(midWin * 1.2f, { 150.f, 75.f }, 20.f, { 2.f, 4.f });
	nar = Narwhal(midWin / 1.3f, { 100.f, 100.f }, 75.f);
	//crab.setDrawType(drawType::BOTH_CR);

	enemies.push_back(&crab);

	stackSprite = StackedObject("./gfx/StackedSpriteTest/cars-1.png", 3.f, { 15,32 });
	stackSprite.setPosition(midWin);
	stackSprite.setSize({ 64.f,128.f });
	stackSprite.setOrigin(stackSprite.getSize() / 2.f);

	/*updateText = new GenericCommand(SUBOA(Button, checkInput, button, window));
	commander.addPressed(sf::Keyboard::Space, updateText);*/
	//buffer actions
	availableActions = {
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->lightAttack(creatures); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->heavyAttack(creatures); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->dodge(); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->parry(); }),
	};

	commander.addPressed(sf::Keyboard::Space, new GenericCommand(SUBA(TestScene, executeAndTrack, availableActions[2])));
	commander.addPressed(sf::Keyboard::Q, new GenericCommand(SUBA(TestScene, executeAndTrack, availableActions[3])));

	commander.addHeld(sf::Keyboard::W, new GenericCommand([=] {player.accelerate({ 0,-1 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::S, new GenericCommand([=] {player.accelerate({ 0,1 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::A, new GenericCommand([=] {player.accelerate({ -1,0 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::D, new GenericCommand([=] {player.accelerate({ 1,0 }, player.getSpeed()); }));
	commander.addPressed(sf::Keyboard::LShift, new GenericCommand([=] {cam.shake(15.f, 0.75f); }));
	commander.addHeld(sf::Keyboard::LControl, new GenericCommand([=] {cam.pan((window->mapPixelToCoords(Input::getIntMousePos()) - g1.getPosition()) * 0.35f); }));
	//commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {commander.swapHeld(sf::Keyboard::W, sf::Keyboard::E); }));
	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {GameState::setCurrentState(State::PAUSE);}));

	cam = Camera(midWin, winSize);
	cam.follow(&player, 0.95f);

	physMan.registerObj(&g1, false);
	physMan.registerObj(&g2, false);
	physMan.registerObj(&player, false);
	physMan.registerObj(&crab, false);
	physMan.registerObj(&nar, false);

	lighter.setTarget(dynamic_cast<sf::RenderTexture*>(window));
	lighter.create();
	lighter.addLight(midWin + sf::Vector2f(-125,150), 250.f, sf::Color::Red);

	//REMOVE WHEN TESTING DYNAMIC LIGHTS
	//lighter.addLight(midWin, 1000.f, sf::Color::White);

	movingLightI = lighter.addLight(midWin - sf::Vector2f(0, midWin.y / 2.f), 250.f, sf::Color::Green);
	movingLight = Light(midWin - sf::Vector2f(0, midWin.y / 2.f), 250.f, sf::Color::Green);


	hue = VectorHelper::RGBtoHSV(sf::Color::Green).x;

	arrow = AssetManager::registerNewTex("arrow");
	arrow->loadFromFile("gfx/debugTestSprites/arrow.png");
	g1.setTexture(arrow);

	AudioManager::createSound("glass", "sfx/Glass_BreakMONO.ogg", 1.0f, false);
	AudioManager::setMaxSoundVol(100.f);

	auto data = SceneDataLoader::loadScene("levels/level.json");
	sceneObjects = data.first;
	for (auto* obj : sceneObjects)
	{
		physMan.registerObj(obj, true);
	}
	for (auto const& light : data.second)
	{
		lighter.addLight(light);
	}

	//load some generic enemies around the screen
	/*for (int i = 0; i < 5; ++i)
	{
		auto* e = new BaseEnemy(midWin + sf::Vector2f(rand() % 1000 - 500, rand() % 1000 - 500), { 50.f,50.f }, 20.f);
		e->setFillColor(sf::Color::Red);
		e->setCollisionShape(cR);
		e->setRotationLock(true);
		e->makeSquareCollisionShape();
		e->setAlive(true);
		enemies.push_back(e);
		physMan.registerObj(e, false);
	}*/
	enemies.push_back(&crab);
	enemies.push_back(&nar);
}

void TestScene::update(float dt)
{
	AudioManager::setListenerPos(g1.getPosition());
	AudioManager::setListenerRot(-90 + ((int)g1.getRotation() % 360));

	r += dt * 15.f;
	std::get<0>(movingLight).x = rock.getPosition().x - 75.f + sin(r/15.f) * 175.f;
	hue += dt * 15.f; if (hue > 360.f) hue -= 360.f;
	std::get<2>(movingLight) = VectorHelper::HSVtoRGB({hue, 1.f, 1.f});
	lighter.setLight(movingLightI, movingLight);

	button.setString(std::to_string(1.f / dt));

	stackSprite.setRotation(r);

	for (auto& e : enemies)
	{
		if (e->isAlive())
		{
			if (dynamic_cast<BaseEnemy*>(e))
			{
				dynamic_cast<BaseEnemy*>(e)->trackPlayer(&player, actionBuffer, dt);
			}
		}
	}

	physMan.update(dt);

	cam.update(dt);
}

void TestScene::handleInput(float dt)
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

	if (Input::isLeftMousePressed())
	{
		auto mp = window->mapPixelToCoords(sf::Vector2i(Input::getMousePos()));
		g1.addForceAtPoint(VectorHelper::normalise(g1.getPosition() + mp) * 500000.f, mp);
		AudioManager::getSound("glass")->playAt(mp);
	}
}

void TestScene::render()
{
	window->setView(cam);

	lighter.beginDraw(sf::Color(130, 112, 148));
	window->draw(button);

	for(auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	//draw enemies with lighter
	for (auto& e : enemies)
	{
		if(e->isAlive())lighter.draw(e);
	}

	lighter.draw(&g1);
	lighter.draw(&g2);
	lighter.draw(&player);

	window->draw(g1.getCollisionShape());
	window->draw(g2.getCollisionShape());
	window->draw(stackSprite);

	lighter.endDraw();
	//for each sceneobject, get its collision shape then for each point in the collision shape, draw a circle at that point
#ifdef DEBUG_COL_POINTS
	for (auto& o : sceneObjects)
	{
		auto s = o->getBaseHull();
		auto c = sf::CircleShape(5.f);
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineColor(sf::Color::Red);
		c.setOutlineThickness(1.f);

		for (size_t i = 0; i < s.getPointCount(); ++i)
		{
			c.setPosition(o->getTransform().transformPoint(s.getPoint(i)) - sf::Vector2f(2.5f, 2.5f));
			window->draw(c);
		}

		window->draw(s);
	}
	auto s = player.getBaseHull();
	auto c = sf::CircleShape(5.f);
	c.setFillColor(sf::Color::Transparent);
	c.setOutlineColor(sf::Color::Red);
	c.setOutlineThickness(1.f);

	for (size_t i = 0; i < s.getPointCount(); ++i)
	{
		c.setPosition(player.getTransform().transformPoint(s.getPoint(i)) - sf::Vector2f(2.5f, 2.5f));
		window->draw(c);
	}

	window->draw(s);

	s = g1.getBaseHull();
	for (size_t i = 0; i < s.getPointCount(); ++i)
	{
		c.setPosition(g1.getTransform().transformPoint(s.getPoint(i)) - sf::Vector2f(2.5f, 2.5f));
		window->draw(c);
	}
#endif // DEBUG_COL_POINTS
}

void TestScene::changeText(const sf::String& msg)
{
	button.setString(msg);
}

void TestScene::executeAndTrack(BufferedCommand* b)
{
	if (!b) return; //if the command is null, do nothing
	if (player.getCooldown() > 0.f) return; //if the player is on cooldown, do nothing

	const int size = actionBuffer.size();
	if (size < maxActBufferSize)
		actionBuffer.push_back(b);
	else
		actionBuffer[oldestAction] = b;

	oldestAction = oldestAction + 1 >= size ? 0 : oldestAction + 1;
	b->execute(nullptr, enemies);
}
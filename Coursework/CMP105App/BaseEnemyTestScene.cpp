#include "BaseEnemyTestScene.h"
//#define DEBUG_COL_POINTS

BaseEnemyTestScene::BaseEnemyTestScene(sf::RenderTarget* hwnd) : Scene(hwnd)
{
	font = AssetManager::registerNewFont("arial");
	font->loadFromFile("./font/arial.ttf");
	button = Button(midWin, winSize * 0.2f, 24, font, "Hello", true);
	button.body().setFillColor(sf::Color::Black);
	button.msg().setFillColor(sf::Color::Cyan);
	button.SUBSCRIBEA(BaseEnemyTestScene, changeText, sf::String("Hello World"));

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

	enemy = BaseEnemy(midWin * 1.2f, { 50, 50 }, 75);
	enemy.setFillColor(sf::Color::Yellow);
	enemy.setRotationLock(true);
	enemy.setCollisionShape(c1);

	stackSprite = StackedObject("./gfx/StackedSpriteTest/cars-1.png", 3.f, { 15,32 });
	stackSprite.setPosition(midWin);
	stackSprite.setSize({ 64.f,128.f });
	stackSprite.setOrigin(stackSprite.getSize() / 2.f);

	updateText = new GenericCommand(SUBOA(Button, checkInput, button, window));
	commander.addPressed(sf::Keyboard::Space, updateText);

	commander.addHeld(sf::Keyboard::W, new GenericCommand([=] {g1.accelerate({ 0,-mSpeed }); }));
	commander.addHeld(sf::Keyboard::S, new GenericCommand([=] {g1.accelerate({ 0,mSpeed }); }));
	commander.addHeld(sf::Keyboard::A, new GenericCommand([=] {g1.accelerate({ -mSpeed,0 }); }));
	commander.addHeld(sf::Keyboard::D, new GenericCommand([=] {g1.accelerate({ mSpeed,0 }); }));
	commander.addPressed(sf::Keyboard::LShift, new GenericCommand([=] {cam.shake(15.f, 0.75f); }));
	commander.addHeld(sf::Keyboard::LControl, new GenericCommand([=] {cam.pan((window->mapPixelToCoords(Input::getIntMousePos()) - g1.getPosition()) * 0.35f); }));
	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {commander.swapHeld(sf::Keyboard::W, sf::Keyboard::E); }));

	cam = Camera(midWin, winSize);
	cam.follow(&g1, 0.95f);

	physMan.registerObj(&g1, false);
	physMan.registerObj(&g2, false);
	physMan.registerObj(&enemy, false);

	lighter.setTarget(dynamic_cast<sf::RenderTexture*>(window));
	lighter.create();
	lighter.addLight(midWin + sf::Vector2f(-125, 150), 250.f, sf::Color::Red);

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
}

void BaseEnemyTestScene::update(float dt)
{
	AudioManager::setListenerPos(g1.getPosition());
	AudioManager::setListenerRot(-90 + ((int)g1.getRotation() % 360));

	r += dt * 15.f;
	std::get<0>(movingLight).x = rock.getPosition().x - 75.f + sin(r / 15.f) * 175.f;
	hue += dt * 15.f; if (hue > 360.f) hue -= 360.f;
	std::get<2>(movingLight) = VectorHelper::HSVtoRGB({ hue, 1.f, 1.f });
	lighter.setLight(movingLightI, movingLight);

	button.setString(std::to_string(1.f / dt));

	stackSprite.setRotation(r);

	physMan.update(dt);

	cam.update(dt);

	//enemy.trackPlayer(&g1, {}, dt);
	//std::cout << enemy.getPosition().x << " ; " << enemy.getPosition().y << std::endl;
}

void BaseEnemyTestScene::handleInput(float dt)
{
	commander.handleInput();
	if (Input::isLeftMousePressed())
	{
		auto mp = window->mapPixelToCoords(sf::Vector2i(Input::getMousePos()));
		g1.addForceAtPoint(VectorHelper::normalise(g1.getPosition() + mp) * 500000.f, mp);
		AudioManager::getSound("glass")->playAt(mp);
	}
}

void BaseEnemyTestScene::render()
{
	window->setView(cam);

	lighter.beginDraw(sf::Color(130, 112, 148));
	window->draw(button);

	for (auto& o : sceneObjects)
	{
		lighter.draw(o);
	}

	lighter.draw(&g1);
	lighter.draw(&g2);
	lighter.draw(&enemy);

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

		auto transform = o->getTransform();
		for (size_t i = 0; i < s.getPointCount(); ++i)
		{
			c.setPosition(o->getTransform().transformPoint(s.getPoint(i)) - sf::Vector2f(2.5f, 2.5f));
			window->draw(c);
		}

		window->draw(s);
	}
#endif // DEBUG_COL_POINTS
}

void BaseEnemyTestScene::changeText(const sf::String& msg)
{
	button.setString(msg);
}

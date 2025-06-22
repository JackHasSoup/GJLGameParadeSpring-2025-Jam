#include "BaseLevel.h"

BaseLevel::BaseLevel()
{
	window = nullptr;
	hitFlashShader = nullptr;

}

BaseLevel::BaseLevel(sf::RenderTarget* hwnd) : Scene(hwnd)
{
	auto* font = AssetManager::registerNewFont("arial");
	font->loadFromFile("./font/arial.ttf");
	// Variable initalisation
	bgColor = sf::Color(130, 112, 148);

	hitFlashShader = AssetManager::registerNewShader("flash");
	if (!hitFlashShader->loadFromFile("shaders/hitFlash.frag", sf::Shader::Type::Fragment)) {
		std::cout << "Error loading hit flash shader";
	}
	hitFlashShader->setUniform("texture", sf::Shader::CurrentTexture);

	// Player
	player = Player(midWin, { 125.f, 125.f }, 20.f);
	physMan.registerObj(&player, false);

	healthBar = HealthBar(window, &player);

	//Door

	door = PhysicsObject((midWin - sf::Vector2f{ 0,400 }), sf::Vector2f{ 200.f,300.f }, 100);
	door.setFillColor(sf::Color::White);
	physMan.registerObj(&door, true);

	// Light Objects

	spotlight = PhysicsObject(midWin, sf::Vector2f{100.f,100.f}, 50);
	sf::CircleShape c = sf::CircleShape(spotlight.getSize().x * 0.44f, 9U); // Circle collision shape referenced from seal player
	sf::ConvexShape lightShape = sf::ConvexShape(c.getPointCount());
	for (int i = 0; i < c.getPointCount(); i++)
	{
		lightShape.setPoint(i, c.getPoint(i) + sf::Vector2f(spotlight.getSize().x * 0.055f, spotlight.getSize().y * 0.052f));
	}
	spotlight.setFillColor(sf::Color::White);
	spotlight.setCollisionShape(lightShape);
	spotlight.setDrawType(drawType::RECT);

	tube = PhysicsObject(midWin, sf::Vector2f{ 175.f,437.5f }, 50.f);

	sf::ConvexShape ovalShape = lightShape;
	for (int i = 0; i < ovalShape.getPointCount(); i++)
	{
		ovalShape.setPoint(i, sf::Vector2f{ lightShape.getPoint(i).x * (tube.getSize().x / 100.f),lightShape.getPoint(i).y * (tube.getSize().y / 250.f) * 0.75f} + sf::Vector2f(0.f, tube.getSize().y * 0.7f));
	}
	tube.setFillColor(sf::Color::White);
	tube.setCollisionShape(ovalShape);
	tube.setDrawType(drawType::RECT_COL_LIGHTMASK);

	availableActions = {
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->lightAttack(creatures); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->heavyAttack(creatures); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->dodge(); }),
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->parry(); }),
	};

	commander.addPressed(sf::Keyboard::Space, new GenericCommand(SUBA(BaseLevel, executeAndTrack, availableActions[2])));
	commander.addPressed(sf::Keyboard::Q, new GenericCommand(SUBA(BaseLevel, executeAndTrack, availableActions[3])));

	commander.addHeld(sf::Keyboard::W, new GenericCommand([=] {player.accelerate({ 0,-1 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::S, new GenericCommand([=] {player.accelerate({ 0,1 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::A, new GenericCommand([=] {player.accelerate({ -1,0 }, player.getSpeed()); }));
	commander.addHeld(sf::Keyboard::D, new GenericCommand([=] {player.accelerate({ 1,0 }, player.getSpeed()); }));

	commander.addPressed(sf::Keyboard::LShift, new GenericCommand([=] {cam.shake(15.f, 0.75f); }));
	commander.addPressed(sf::Keyboard::LAlt, new GenericCommand([=] {GameState::incrementLevel(); }));

	commander.addPressed(sf::Keyboard::Escape, new GenericCommand([=] {GameState::setCurrentState(State::PAUSE); }));

	// Camera + light
	cam = Camera(midWin, winSize);
	cam.follow(&player, 0.95f);

	lighter.setTarget(dynamic_cast<sf::RenderTexture*>(window));
	lighter.create();

}

void BaseLevel::reset()
{
	player.restoreHealth();
	player.setHitTimer(0.f);
	player.setJumpTime(0.f);
	player.positionReset(door.getPosition() + sf::Vector2f{ 0.f, door.getSize().y * 1.25f });
	player.setHitTimer(0.f);
	cam.setCenter(door.getPosition());

	actionBuffer.clear();

	for (int i = 0; i < rooms.size(); i++) {
		rooms[i].setAllCreaturesDead(false);
		for (int j = 0; j < rooms[i].getCreatures().size(); j++) {
			// For all creatures in all rooms set alive and heal to full
			rooms[i].getCreatures().at(j)->restoreHealth();
			rooms[i].getCreatures().at(j)->positionReset(rooms[i].getCreatures().at(j)->getBasePos()); // reset position to what it was set to
			rooms[i].getCreatures().at(j)->setAlive(true);
			rooms[i].getCreatures().at(j)->setHitTimer(0.f);
			rooms[i].getCreatures().at(j)->setCooldown(rooms[i].getCreatures().at(j)->getMaxCooldown());
			dynamic_cast<BaseEnemy*>(rooms[i].getCreatures().at(j))->resetBuffer();
			dynamic_cast<CreatureObject*>(rooms[i].getCreatures().at(j));
		}
	}

	std::get<0>(doorLight) = sf::Vector2f{ door.getPosition() - sf::Vector2f(-23.f, 70.f) };
	std::get<1>(doorLight) = 100.f;
	std::get<2>(doorLight) = sf::Color::Red;
	lighter.setLight(doorLightI, doorLight);

}

void BaseLevel::loadLevel(std::string const& filename)
{

	auto data = SceneDataLoader::loadScene(filename);
	sceneObjects =std::get<0>(data);
	for (auto* obj : sceneObjects)
	{
		physMan.registerObj(obj, true);
	}
	for (auto const& light : std::get<1>(data))
	{
		lighter.addLight(light);
	}
	for (auto const& room : std::get<2>(data))
	{
		rooms.push_back(Room(room, &player));
	}
	for (auto const& creature : std::get<3>(data))
	{
		auto [creatureType, position, roomIndex] = creature;

		CreatureObject* newCreature = nullptr;
		switch (creatureType)
		{
		case EditorCreature::PLAYER:
			player.positionReset(position);
			continue;
		case EditorCreature::CRAB:
			newCreature = new Crab(position, { 300.f, 155.f }, 20.f, { 0.f,1.f }); //MAKE SURE YOU EDIT THE CRABS DIRECTION MANUALLY!!!!!!!!
			break;
		case EditorCreature::NARWHAL:
			newCreature = new Narwhal(position, { 200.f, 200.f }, 75.f);
			break;
		case EditorCreature::JELLYFISH:
			newCreature = new Jellyfish(position, { 300.f, 300.f }, 20.f);
			break;
		case EditorCreature::WALRUS:
			newCreature = new Walrus(position, { 400.f, 400.f }, 40.f);
			break;
		default:
			continue; // skip unknown creature types
		}

		if (roomIndex >= 0 && roomIndex < rooms.size())
		{
			rooms[roomIndex].addCreature(newCreature);
		}

		if (newCreature)
		{
			physMan.registerObj(newCreature, false);
		}
	}

	floorTexture->setRepeated(true);

	for (int i = 0; i < rooms.size(); i++) {
		GameObject* newFloor = new GameObject();
		newFloor->setSize(sf::Vector2f{ rooms[i].width, rooms[i].height });
		newFloor->setOrigin(newFloor->getSize() / 2.f);
		newFloor->setPosition(sf::Vector2f{ (rooms[i].left + (rooms[i].width / 2.f)), (rooms[i].top + (rooms[i].height / 2.f)) });
		newFloor->setTexture(floorTexture);
		newFloor->setTextureRect(sf::IntRect(0, 0, newFloor->getSize().x, newFloor->getSize().y));
		floors.push_back(newFloor);
	}

	for (int i = 0; i < rooms.size(); i++) {
		// For each room have a black rectangle that blocks it out when not active
		GameObject* rect = new GameObject();
		rect->setSize(floors[i]->getSize());
		rect->setOrigin(floors[i]->getOrigin());
		rect->setPosition(floors[i]->getPosition());
		rect->setTexture(fogTexture);
		rect->setFillColor(sf::Color::White);
		roomFog.push_back(rect);
	}

	door.setPosition(player.getPosition() - sf::Vector2f{0.f,door.getSize().y * 1.25f});
	door.setTexture(doorTexture);

	doorLightI = lighter.addLight(door.getPosition() - sf::Vector2f(-23.f, 70.f), 100.f, sf::Color::Red);
	doorLight = Light(door.getPosition() - sf::Vector2f(0, 40.f), 50.f, sf::Color::Red);

	spotlight.setTexture(spotlightTexture);
	tube.setTexture(tubeTexture);

	cam.setCenter(door.getPosition());


}

void BaseLevel::onEnter(Player* inputPlayer)
{
	// Once all levels are baseLevels, send the player data to the next level

}

void BaseLevel::render()
{
}

void BaseLevel::doorCheck()
{

	for (int i = 0; i < rooms.size(); i++) {
		if (!rooms[i].allCreaturesDead()) {
			// If any room still have creatures then door is not oepn
			return;
		}
	}

	// change door light to green 
	std::get<0>(doorLight) = sf::Vector2f{ door.getPosition() - sf::Vector2f(-23.f, 70.f) };
	std::get<1>(doorLight) = 100.f;
	std::get<2>(doorLight) = sf::Color::Green;
	lighter.setLight(doorLightI, doorLight);

	if (Collision::checkBoundingBox(&player, &door)) {
		GameState::incrementLevel();
	}

}

void BaseLevel::executeAndTrack(BufferedCommand* b)
{
	if (!b) return; //if the command is null, do nothing
	if (player.getCooldown() > 0.f) return; //if the player is on cooldown, do nothing

	const int size = actionBuffer.size();
	if (size < maxActBufferSize)
		actionBuffer.push_back(b);
	else
		actionBuffer[oldestAction] = b;

	oldestAction = oldestAction + 1 >= size ? 0 : oldestAction + 1;
	b->execute(nullptr, activeRoom->getCreatures());
}

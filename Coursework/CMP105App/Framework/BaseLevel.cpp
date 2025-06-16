#include "BaseLevel.h"

BaseLevel::BaseLevel()
{
	window = nullptr;
}

BaseLevel::BaseLevel(sf::RenderTarget* hwnd) : Scene(hwnd)
{

	// Variable initalisation
	enemyCount = 0;
	bgColor = sf::Color(130, 112, 148);

	// Player
	player = Player(midWin, { 75.f, 75.f }, 20.f);

	physMan.registerObj(&player, false);

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

void BaseLevel::loadLevel(std::string const& filename)
{

	auto data = SceneDataLoader::loadScene(filename);
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

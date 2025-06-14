#include "StoredMoveBufferTestScene.h"

StoredMoveBufferTestScene::StoredMoveBufferTestScene(sf::RenderTarget* hwnd) : Scene(hwnd)
{
	cam = Camera(midWin, winSize);
	player = BaseEnemy(midWin, { 50.f, 50.f }, 10.f);
	enemyStandin = BaseEnemy(midWin + sf::Vector2f(0, -150.f), { 50.f, 50.f }, 10.f);
	enemyStandin.setFillColor(sf::Color::Yellow);
	enemyStandin.setRotationLock(true);
	
	availableActions = {
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->accelerate({ 0.f, -35000.f }); }),//up
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->accelerate({ 0.f, 35000.f }); }),//down
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->accelerate({ -35000.f, 0.f }); }),//left
		new BufferedCommand(&player, [](CreatureObject* target, std::vector<CreatureObject*> creatures) {target->accelerate({ 35000.f, 0.f }); })//right
	};

	cmndr.addPressed(sf::Keyboard::W, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[0])));
	cmndr.addPressed(sf::Keyboard::S, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[1])));
	cmndr.addPressed(sf::Keyboard::A, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[2])));
	cmndr.addPressed(sf::Keyboard::D, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[3])));
}

void StoredMoveBufferTestScene::update(float dt)
{
	//no physMan, no collision irrelevant in this scenario
	enemyStandin.trackPlayer(&player, actionList, dt);

	player.update(dt);
	enemyStandin.update(dt);
	cam.update(dt);
}

void StoredMoveBufferTestScene::handleInput(float dt)
{
	cmndr.handleInput();

	cooldown -= dt;
	const int size = actionList.size();
	if (cooldown <= 0.f && size)
	{
		cooldown = maxCooldown;
		actionList[performingAction]->execute(&enemyStandin, {});

		performingAction = performingAction + 1 >= size ? 0 : performingAction + 1;
	}
}

void StoredMoveBufferTestScene::render()
{
	window->setView(cam);
	beginDraw();
	window->draw(player);
	window->draw(enemyStandin);
}

void StoredMoveBufferTestScene::executeAndTrack(BufferedCommand* b)
{
	const int size = actionList.size();
	if (size < maxActions)
		actionList.push_back(b);
	else
		actionList[oldestAction] = b;
	
	oldestAction = oldestAction+1 >= size ? 0 : oldestAction+1;
	b->execute();
}

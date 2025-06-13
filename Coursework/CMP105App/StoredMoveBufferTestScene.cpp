#include "StoredMoveBufferTestScene.h"

StoredMoveBufferTestScene::StoredMoveBufferTestScene(sf::RenderTarget* hwnd)
{
	cam = Camera(midWin, winSize);
	player = PhysicsObject(midWin, { 50.f, 50.f }, 10.f);
	actionList.resize(6);
	
	availableActions = {
		new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 0.f, -10.f }); }),//up
		new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 0.f, 10.f }); }),//down
		new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ -10.f, 0.f }); }),//left
		new BufferedCommand(&player, [](PhysicsObject* target) {target->accelerate({ 10.f, 0.f }); })//right
	};

	cmndr.addPressed(sf::Keyboard::W, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[0])));
	cmndr.addPressed(sf::Keyboard::S, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[1])));
	cmndr.addPressed(sf::Keyboard::A, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[2])));
	cmndr.addPressed(sf::Keyboard::D, new GenericCommand(SUBA(StoredMoveBufferTestScene, executeAndTrack, availableActions[3])));
}

void StoredMoveBufferTestScene::update(float dt)
{
	player.update(dt);
}

void StoredMoveBufferTestScene::handleInput(float dt)
{
	cmndr.handleInput();
}

void StoredMoveBufferTestScene::render()
{
	beginDraw();
	window->draw(player);
}

void StoredMoveBufferTestScene::executeAndTrack(BufferedCommand* b)
{
	actionList[oldestAction] = b;
	oldestAction = oldestAction+1 >= maxActions ? 0 : oldestAction+1;
	b->execute();
}

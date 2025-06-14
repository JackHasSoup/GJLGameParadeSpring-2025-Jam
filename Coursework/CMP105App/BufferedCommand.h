#pragma once
#include <functional>
#include "Framework/Command.h"
#include "CreatureObject.h"

class BufferedCommand : public Command
{
public:
	BufferedCommand(CreatureObject* defaultTarget, std::function<void(CreatureObject* target, std::vector<CreatureObject*> creatures)> cb);
	~BufferedCommand() {};

	virtual void execute(CreatureObject* target, std::vector<CreatureObject*> creatures);
	virtual void execute() override { execute(nullptr, {}); };
private:
	std::function<void(CreatureObject* target, std::vector<CreatureObject*> creatures)> callback;
	CreatureObject* defaultTarget;
};
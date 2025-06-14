#pragma once
#include <functional>
#include "Framework/Command.h"
#include "CreatureObject.h"

class BufferedCommand : public Command
{
public:
	BufferedCommand(CreatureObject* defaultTarget, std::function<void(CreatureObject* target)> cb);
	~BufferedCommand() {};

	virtual void execute(CreatureObject* target);
	virtual void execute() override { execute(nullptr); };
private:
	std::function<void(CreatureObject* target)> callback;
	CreatureObject* defaultTarget;
};
#pragma once
#include <functional>
#include "Framework/Command.h"
#include "Framework/PhysicsObject.h"

class BufferedCommand : public Command
{
public:
	BufferedCommand(PhysicsObject* defaultTarget, std::function<void(PhysicsObject* target)> cb);
	~BufferedCommand() {};

	virtual void execute(PhysicsObject* target);
	virtual void execute() override { execute(nullptr); };
private:
	std::function<void(PhysicsObject* target)> callback;
	PhysicsObject* defaultTarget;
};
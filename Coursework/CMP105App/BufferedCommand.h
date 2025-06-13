#pragma once
#include <functional>
#include "Framework/Command.h"
#include "Framework/PhysicsObject.h"

class BufferedCommand : public Command
{
	BufferedCommand(PhysicsObject* defaultTarget, std::function<void(PhysicsObject* target)> cb);
	~BufferedCommand() {};

	virtual void execute(PhysicsObject* target = nullptr);
private:
	std::function<void(PhysicsObject* target)> callback;
	PhysicsObject* defaultTarget;
};
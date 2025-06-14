#include "BufferedCommand.h"

BufferedCommand::BufferedCommand(PhysicsObject* defaultTarget, std::function<void(PhysicsObject* target)> cb)
{
	this->defaultTarget = defaultTarget;
	callback = cb;
}

void BufferedCommand::execute(PhysicsObject* target)
{
	callback(target ? target : defaultTarget);
}

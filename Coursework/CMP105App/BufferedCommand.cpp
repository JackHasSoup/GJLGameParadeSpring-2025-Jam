#include "BufferedCommand.h"

BufferedCommand::BufferedCommand(CreatureObject* defaultTarget, std::function<void(CreatureObject* target)> cb)
{
	this->defaultTarget = defaultTarget;
	callback = cb;
}

void BufferedCommand::execute(CreatureObject* target)
{
	callback(target ? target : defaultTarget);
}

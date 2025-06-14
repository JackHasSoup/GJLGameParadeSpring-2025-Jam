#include "BufferedCommand.h"

BufferedCommand::BufferedCommand(CreatureObject* defaultTarget, std::function<void(CreatureObject* target, std::vector<CreatureObject*> creatures)> cb)
{
	this->defaultTarget = defaultTarget;
	callback = cb;
}

void BufferedCommand::execute(CreatureObject* target, std::vector<CreatureObject*> creatures)
{
	callback(target ? target : defaultTarget, creatures);
}

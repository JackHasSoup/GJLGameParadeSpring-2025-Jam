#include "GenericCommand.h"

GenericCommand::GenericCommand(std::function<void()> cb)
{
	callback = cb;
}

void GenericCommand::execute()
{
	callback();
}

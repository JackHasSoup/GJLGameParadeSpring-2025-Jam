#pragma once

class Command 
{
public:
	Command() {};
	virtual void execute() = 0;
};
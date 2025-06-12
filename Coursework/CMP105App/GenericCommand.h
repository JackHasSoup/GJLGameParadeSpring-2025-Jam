#pragma once
#include "Framework/Command.h"
#include <functional>

#ifndef SUB(a,b) 

#define SUB(a,b) std::bind(&a::b, this)
#define SUBA(a,b,c) std::bind(&a::b, this, c)
#define SUBO(a,b,c) std::bind(&a::b, &c)
#define SUBOA(a,b,c,d) std::bind(&a::b, &c, d)

#endif // !SUBSCRIBE(a,b) 

class GenericCommand : public Command
{
public:
	GenericCommand(std::function<void()> cb);
	~GenericCommand() {};

	virtual void execute() override;
private:
	std::function<void()> callback;
};

/*
* the most basic, generic, and flexible command
* just give it a function to execute
*/


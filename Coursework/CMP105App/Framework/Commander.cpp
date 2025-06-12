#include "Commander.h"

Commander::~Commander()
{
	for (auto& pair : pressedCmnds)
	{
		delete pair.second;
	}

	for (auto& pair : heldCmnds)
	{
		delete pair.second;
	}
}

void Commander::addPressed(int keycode, Command* command)
{
	pressedCmnds.insert({ keycode, command });
}

void Commander::addHeld(int keycode, Command* command)
{
	heldCmnds.insert({ keycode, command });
}

void Commander::removePressed(int keycode)
{
	delete pressedCmnds[keycode];
	pressedCmnds.erase(keycode);
}

void Commander::removeHeld(int keycode)
{
	delete heldCmnds[keycode];
	heldCmnds.erase(keycode);
}

void Commander::executePressed(int keycode)
{
	//nullptr check
	if(pressedCmnds[keycode]) pressedCmnds[keycode]->execute();
}

void Commander::executeHeld(int keycode)
{
	//nullptr check
	if (heldCmnds[keycode]) heldCmnds[keycode]->execute();
}

void Commander::swapPressed(int k1, int k2)
{
	Command** a = &pressedCmnds[k1];
	Command** b = &pressedCmnds[k2];
	Command* t = *a;

	*a = *b;
	*b = t;
}

void Commander::swapHeld(int k1, int k2)
{
	Command** a = &heldCmnds[k1];
	Command** b = &heldCmnds[k2];
	Command* t = *a;

	*a = *b;
	*b = t;
}

void Commander::handleInput()
{
	for (auto& cmnd : pressedCmnds)
	{
		if (Input::isPressed(cmnd.first) && cmnd.second) cmnd.second->execute();
	}

	for (auto& cmnd : heldCmnds)
	{
		if (Input::isKeyDown(cmnd.first) && cmnd.second) cmnd.second->execute();
	}
}

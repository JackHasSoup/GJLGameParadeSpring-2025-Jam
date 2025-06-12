#pragma once
#include <map>
#include "Input.h"
#include <iostream>
#include "Command.h"

class Commander
{
public:
	Commander() {};
	~Commander();
	//registers a new command with its associated keycode
	void addPressed(int keycode, Command* command);
	void addHeld(int keycode, Command* command);

	//removes a command from the executable list
	void removePressed(int keycode);
	void removeHeld(int keycode);

	//executes the command associated with the passed keycode
	void executePressed(int keycode);
	void executeHeld(int keycode);

	//returns the command associated with an input
	Command** getPressedCommand(int k) { return &pressedCmnds[k]; };
	Command** getHeldCommand(int k) { return &heldCmnds[k]; };
	
	//swaps two commands
	void swapPressed(int k1, int k2);
	void swapHeld(int k1, int k2);

	void handleInput();
private:
	std::map<int, Command*> pressedCmnds;
	std::map<int, Command*> heldCmnds;
};


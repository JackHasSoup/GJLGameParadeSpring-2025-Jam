#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

class Input
{
public:
	enum class MouseState { UP, DOWN, PRESSED };
private:
	//mouse structure, store mouse relevant information
	

	struct Mouse
	{
		int x, y;
		MouseState left, right;
	};
	

public:
	Input() {};
	static void init();
	//functions for setting key as pressed, released and checking current state.
	static void setKeyDown(int key);
	static void setKeyUp(int key);
	static bool isKeyDown(int key);
	static bool isPressed(int key);
	static void update();

	//functions are mouse input, including getting and setting current position and mouse button presses.
	static void setMouseX(int lx);
	static void setMouseY(int ly);
	static void setMousePosition(int lx, int ly);
	static void setScrollDelta(float const& delta);
	static int getMouseX();
	static int getMouseY();
	static sf::Vector2f getMousePos();
	static sf::Vector2i getIntMousePos();
	static float getScrollDelta();
	static void setLeftMouse(MouseState state);
	static bool isLeftMouseDown();
	static bool isLeftMousePressed();
	static void setRightMouse(MouseState state);
	static bool isRightMouseDown();
	static bool isRightMousePressed();

	static void handleEvents(sf::RenderWindow* const window);
	
private:
	//array of booleans representing keys (pressed = true, released = false)
	static bool keys[256];
	static std::vector<int> pressed;
	//mouse variable
	static Mouse mouse;
	static float scrollDelta;
};

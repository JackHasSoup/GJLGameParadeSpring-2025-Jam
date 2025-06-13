#include "Input.h"
bool Input::keys[256] = { false };
std::vector<int> Input::pressed = {};
Input::Mouse Input::mouse = {0.f, 0.f, Input::MouseState::UP ,Input::MouseState::UP };
float Input::scrollDelta = 0.f;

void Input::init()
{
	// set default values
	Input::mouse.left = MouseState::UP;
	Input::mouse.right = MouseState::UP;
	Input::mouse.x = 0;
	Input::mouse.y = 0;
}

void Input::setKeyDown(int key)
{
	if (key >= 0)
	{
		Input::keys[key] = true;
	}
}

void Input::setKeyUp(int key)
{
	if (key >= 0)
	{
		Input::keys[key] = false;
	}
}

bool Input::isKeyDown(int key)
{
	if (key >= 0)
	{
		return Input::keys[key];
	}
	return false;
}

bool Input::isPressed(int key)
{
	bool cond = Input::isKeyDown(key);
	if (cond)
	{
		Input::pressed.push_back(key);
		return cond;
	}
	return false;
}

void Input::update()
{
	for (int i = 0; i < Input::pressed.size(); i++)
	{
		Input::setKeyUp(Input::pressed[i]);
	}
	Input::pressed.clear();

	if (Input::mouse.left == MouseState::PRESSED)
	{
		Input::mouse.left = MouseState::DOWN;
	}
	if (Input::mouse.right == MouseState::PRESSED)
	{
		Input::mouse.right = MouseState::DOWN;
	}
}

void Input::setMouseX(int lx)
{
	Input::mouse.x = lx;
}

void Input::setMouseY(int ly)
{
	Input::mouse.y = ly;
}

void Input::setMousePosition(int lx, int ly)
{
	Input::setMouseX(lx);
	Input::setMouseY(ly);
}

void Input::setScrollDelta(float const& delta)
{
	scrollDelta = delta;
}

int Input::getMouseX()
{
	return Input::mouse.x;
}

int Input::getMouseY()
{
	return Input::mouse.y;
}

sf::Vector2f Input::getMousePos()
{
	return sf::Vector2f(Input::getMouseX(), Input::getMouseY());
}

sf::Vector2i Input::getIntMousePos()
{
	return sf::Vector2i(Input::getMouseX(), Input::getMouseY());
}

float Input::getScrollDelta()
{
	return scrollDelta;
}

void Input::setLeftMouse(MouseState state)
{
	Input::mouse.left = state;
}
bool Input::isLeftMouseDown()
{
	if (Input::mouse.left == MouseState::DOWN )
	{
		return true;
	}
	return false;
}

bool Input::isLeftMousePressed()
{
	if (Input::mouse.left == MouseState::PRESSED)
	{

		return true;
	}
	return false;
}

void Input::setRightMouse(MouseState state)
{
	Input::mouse.right = state;
}
bool Input::isRightMouseDown()
{
	if (Input::mouse.right == MouseState::DOWN )
	{
		return true;
	}
	return false;
}

bool Input::isRightMousePressed()
{
	if (Input::mouse.right == MouseState::PRESSED)
	{
		
		return true;
	}
	return false;
}

void Input::handleEvents(sf::RenderWindow* const window, sf::RenderTexture* tex)
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			if (tex) { tex->create((float)event.size.width, (float)event.size.height); }
			break;
		case sf::Event::KeyPressed:
			// update input class
			Input::setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			Input::setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			Input::setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				Input::setLeftMouse(Input::MouseState::PRESSED);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				Input::setRightMouse(Input::MouseState::PRESSED);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				Input::setLeftMouse(Input::MouseState::UP);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				Input::setRightMouse(Input::MouseState::UP);
			}
			break;
		case sf::Event::MouseWheelScrolled:
			Input::setScrollDelta(event.mouseWheelScroll.delta == Input::getScrollDelta() ? 0 : event.mouseWheelScroll.delta);
			break;
		default:
			// don't handle other events
			break;
		}
	}
}


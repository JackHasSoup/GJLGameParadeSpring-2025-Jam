#pragma once
#include "SFML/Graphics.hpp"
#include "Input.h"
#include <iostream>
#include <functional>

#ifndef SUBSCRIBE(a,b) 

#define SUBSCRIBE(a,b) subscribe(std::bind(&a::b, this))
#define SUBSCRIBEA(a,b,c) subscribe(std::bind(&a::b, this, c))

#endif // !SUBSCRIBE(a,b) 
//macro to make subscribing to a button easier
//simply type [BUTTON_NAME].SUBSCRIBE(className, functionName);
// or [BUTTON_NAME].SUBSCRIBE(className, functionName, arg); where arg is any type that could pass to the function

class Button : public sf::Drawable
{
public:
	Button(
		const sf::Vector2f& pos,
		const sf::Vector2f& size,
		const float& fontSize,
		const sf::Font* font,
		const sf::String& msg,
		bool centre = true
	);
	Button() {};
	~Button() {};

	//checks to see if the button has been clicked
	virtual void checkInput(sf::RenderTarget* window);

	//returns a reference to the buttons text, for ease of manipulation
	virtual sf::Text& msg() { return std::ref(text); };

	//set the string of the button
	virtual void setString(const sf::String& str);

	//functions to make it feel like button is a rect inherently
	virtual sf::RectangleShape& body() { return std::ref(rect); };

	//adds a ptr to a function the callback stack
	virtual void subscribe(std::function<void()> callback);
protected:
	bool clicked = false;
	sf::Text text;
	sf::RectangleShape rect;
	std::vector<std::function<void()>> subscribers;

	//called when you use window->draw(button). hooks text into the rendering of the button
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


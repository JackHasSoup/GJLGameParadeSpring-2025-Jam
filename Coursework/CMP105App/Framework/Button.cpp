#include "Button.h"
#define mPos window->mapPixelToCoords(Input::getIntMousePos())

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size, const float& fontSize, const sf::Font* font, const sf::String& msg, bool centre)
{
	rect.setPosition(pos);
	rect.setSize(size);

	if (centre) rect.setOrigin(size / 2.f);

	text = sf::Text("", *font, fontSize);
	setString(msg);
}

void Button::checkInput(sf::RenderTarget* window)
{
	//make sure to wrap checkInput inside a is left mouse clicked clause first. group all buttons together in the same if please
	if (rect.getGlobalBounds().contains(mPos))
	{
		for(auto& cb : subscribers)
		{
			cb();
		}
	}
}

void Button::setString(const sf::String& str)
{
	if (str == text.getString()) return;

	text.setString(str);
	text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
	text.setPosition(rect.getPosition());
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
	target.draw(text, states);
}

void Button::subscribe(std::function<void()> callback)
{
	subscribers.push_back(callback);
}

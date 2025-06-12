#include "Animation.h"

Animation::Animation()
{
	currentFrame = 0;
	elapsedTime = 0.f;
	isPlaying = true;
	isLooping = true;
	animationSpeed = 0.0f;
	flipped = false;
}

void Animation::addFrame(sf::IntRect rect)
{
	frames.push_back(rect);
}

int Animation::getSize()
{
	return (int)frames.size();
}

sf::IntRect Animation::getCurrentFrame()
{
	frame = frames[currentFrame];
	if (flipped)
	{

		frame = sf::IntRect(frame.left + frame.width, frame.top, -frame.width, frame.height);
	}
	return frame;
}

void Animation::animate(float dt)
{
	if (isPlaying)
	{
		elapsedTime += dt;
		if (elapsedTime >= animationSpeed)
		{
			currentFrame++;
			if (currentFrame >= (int)frames.size())
			{
				if (isLooping)
				{
					currentFrame = 0;
				}
				else
				{
					currentFrame--;
					setPlaying(false);
				}
			}
			elapsedTime = 0;
		}
	}
}

void Animation::reset()
{
	currentFrame = 0;
	elapsedTime = 0;
}

void Animation::stop()
{
	setPlaying(false);
	currentFrame = 0;
	elapsedTime = 0;
}

void Animation::setFrameSpeed(float speed)
{
	animationSpeed = speed;
}
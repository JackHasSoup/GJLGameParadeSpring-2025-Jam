#include "Sound.h"

Sound::Sound(sf::String const& path, float const& volume, float const& maxVolume, bool const& looping)
{
	this->volume = volume;
	buffer.loadFromFile(path);
	sound.setBuffer(buffer);
	sound.setVolume(volume * maxVolume);
	sound.setLoop(looping);
}

void Sound::updateVolume(float const& newMax, float const& v)
{
	if (v) volume = v;
	sound.setVolume( volume * newMax);
}

void Sound::play()
{
	sound.setPosition({});
	sound.setRelativeToListener(true);
	sound.play();
}

void Sound::playAt(sf::Vector2f const& p)
{
	sound.setPosition(p.x, 0.f, p.y);
	sound.setRelativeToListener(false);
	sound.setMinDistance(25.f);
	sound.setAttenuation(5.f);
	sound.play();
}

#include "AudioManager.h"
float AudioManager::maxSoundVolume = 0.f;
float AudioManager::maxMusicVolume = 0.f;
std::map<sf::String, Sound*> AudioManager::sounds;
sf::Music AudioManager::music;

void AudioManager::init()
{
	maxSoundVolume = 100.0;
	maxMusicVolume = 35.0;
}

void AudioManager::setListenerPos(sf::Vector2f const& p)
{
	sf::Listener::setPosition(p.x, 0.f, p.y);
}

//angle must be in degrees
void AudioManager::setListenerRot(float const& angle)
{
	const auto v = VectorHelper::rotateDegrees({1.f,0.f}, angle);
	sf::Listener::setDirection(v.x, 0.f, v.y);
}

void AudioManager::setListenerDir(sf::Vector2f const& d)
{
	sf::Listener::setDirection(d.x, 0.f, d.y);
}

//where 0 < volume <= 1 to set this sounds volume as a percentage of max volume
Sound* AudioManager::createSound(sf::String const& name, sf::String const& path, float const& volume, bool const& looping)
{
	Sound* s = new Sound(path, volume, maxSoundVolume, looping);
	sounds.emplace(name, s);
	return s;
}

Sound* AudioManager::getSound(sf::String const& name)
{
	return sounds[name];
}

void AudioManager::playMusic(sf::String const& path, bool const& looping)
{
	music.openFromFile(path);
	music.setLoop(looping);
}

void AudioManager::setMaxSoundVol(float const& V)
{
	maxSoundVolume = V;
	sf::Listener::setGlobalVolume(maxSoundVolume);
	for (auto& s : sounds)
	{
		s.second->updateVolume(maxSoundVolume);
	}
}

void AudioManager::setmaxMusicVol(float const& V)
{
	maxMusicVolume = V;
	music.setVolume(maxMusicVolume);
}

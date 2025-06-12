#pragma once
#include "Sound.h"
#include "VectorHelper.h"
#include <iostream>
#include <map>
#include <algorithm>

//custom spatialised audio wrapper
class AudioManager {
public:
	static void init();

	static void setListenerPos(sf::Vector2f const& p);
	static void setListenerRot(float const& angle);
	static void setListenerDir(sf::Vector2f const& d);

	static void playMusic(sf::String const& path, bool const& looping);

	static void setMaxSoundVol(float const& V);
	static void setmaxMusicVol(float const& V);

	static Sound* createSound(sf::String const& name, sf::String const& path, float const& volume, bool const& looping);
	static Sound* getSound(sf::String const& name);

private:
	static float maxSoundVolume, maxMusicVolume;
	static std::map<sf::String, Sound*> sounds;

	static sf::Music music;
};
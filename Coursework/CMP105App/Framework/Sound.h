#include "SFML/Audio.hpp"

class Sound {
public:
	Sound(sf::String const& path, float const& volume, float const& maxVolume, bool const& looping);
	~Sound() {};

	void updateVolume(float const& newMax, float const& v = false);
	void setLooping(bool const& b) { sound.setLoop(b); };
	sf::Sound* getSound() { return &sound; }
	sf::SoundBuffer* getBuffer() { return &buffer; }
	void play();
	void playAt(sf::Vector2f const& p);
private:
	sf::Sound sound;
	sf::SoundBuffer buffer;
	float volume = 1.f;
};
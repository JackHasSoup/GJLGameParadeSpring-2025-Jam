#pragma once
#include "SFML/Graphics.hpp"
#include <map>

//static class for keeping track of all assets in the game
class AssetManager {
public:
	AssetManager() {};
	~AssetManager() {};//no need to cleanup, this class goes out of scope only when game closes
	static void registerExisting(sf::String const& key, sf::Texture* asset);
	static void registerExisting(sf::String const& key, sf::Font* asset);
	static void registerExisting(sf::String const& key, sf::Shader* asset);

	static sf::Texture* registerNewTex(sf::String const& key);
	static sf::Font* registerNewFont(sf::String const& key);
	static sf::Shader* registerNewShader(sf::String const& key);

	static sf::Texture* getTex(sf::String const& key);
	static sf::Font* getFont(sf::String const& key);
	static sf::Shader* getShader(sf::String const& key);

	static std::map<sf::String, sf::Texture*>& getAllTextures() { return textures; }
	static std::map<sf::String, sf::Font*>& getAllFonts() { return fonts; }
	static std::map<sf::String, sf::Shader*>& getAllShaders() { return shaders; }
private:
	static std::map<sf::String, sf::Texture*> textures;
	static std::map<sf::String, sf::Font*> fonts;
	static std::map<sf::String, sf::Shader*> shaders;
};
#include "AssetManager.h"
std::map<sf::String, sf::Texture*> AssetManager::textures;
std::map<sf::String, sf::Font*> AssetManager::fonts;
std::map<sf::String, sf::Shader*> AssetManager::shaders;

void AssetManager::registerExisting(sf::String const& key, sf::Texture* asset)
{
	if (textures[key]) return; //don't double add asset

	textures.emplace(key, asset);
}

void AssetManager::registerExisting(sf::String const& key, sf::Font* asset)
{
	if (fonts[key]) return; //don't double add asset

	fonts.emplace(key, asset);
}

void AssetManager::registerExisting(sf::String const& key, sf::Shader* asset)
{
	if (shaders[key]) return; //don't double add asset

	shaders.emplace(key, asset);
}

sf::Texture* AssetManager::registerNewTex(sf::String const& key)
{
	if (textures.count(key)) return textures[key]; //don't double add asset

	sf::Texture* a = new sf::Texture();
	textures.emplace(key, a);
	return a;
}

sf::Font* AssetManager::registerNewFont(sf::String const& key)
{
	if (fonts.count(key)) return fonts[key]; //don't double add asset

	sf::Font* a = new sf::Font();
	fonts.emplace(key, a);
	return a;
}

sf::Shader* AssetManager::registerNewShader(sf::String const& key)
{
	if (shaders.count(key)) return shaders[key]; //don't double add asset

	sf::Shader* a = new sf::Shader();
	shaders.emplace(key, a);
	return a;
}

sf::Texture* AssetManager::getTex(sf::String const& key)
{
	return textures[key];
}

sf::Font* AssetManager::getFont(sf::String const& key)
{
	return fonts[key];
}

sf::Shader* AssetManager::getShader(sf::String const& key)
{
	return shaders[key];
}

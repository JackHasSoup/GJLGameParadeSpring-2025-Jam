#pragma once
#include "../Framework/PhysicsObject.h"
#include "../Framework/DeferredIllumination.h"
#include "../Framework/AssetManager.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

enum class EditorCreature {//supports 10 creatures!!!!
	UNKNOWN = -1, // used for unassigned creatures
	PLAYER = 0,
	CRAB = 1,
	NARWHAL = 2,
	JELLYFISH = 3,
	WALRUS = 4
};

#ifndef SceneData
#define SceneData std::tuple<std::vector<PhysicsObject*>, std::vector<Light>, std::vector<sf::FloatRect>, std::vector<std::tuple<EditorCreature, sf::Vector2f, int>>>
#endif // !SceneData


class SceneDataLoader {
public:
	static SceneData loadScene(std::string const& filename);

	static void setColour(PhysicsObject* obj, sf::Color const& c);
	static void setOutlineColour(PhysicsObject* obj, sf::Color const& c, float const& thickness = -1.f);
	static void setTexture(PhysicsObject* obj, sf::String const& name);

	//helper for sf::Color
	static json colourToJson(const sf::Color& c) {
		return { {"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a} };
	}
	static sf::Color colourFromJson(const json& j) {
		return sf::Color(j.value("r", 255), j.value("g", 255), j.value("b", 255), j.value("a", 255));
	}

	//helper for sf::Vector2f
	static json vecToJson(const sf::Vector2f& v) {
		return { {"x", v.x}, {"y", v.y} };
	}
	static sf::Vector2f vecFromJson(const json& j) {
		return sf::Vector2f(j.value("x", 0.f), j.value("y", 0.f));
	}
private:
};
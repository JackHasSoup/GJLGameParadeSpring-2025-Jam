#pragma once
#include "../Framework/Scene.h"
#include "../Framework/PhysicsObject.h"
#include "../GenericCommand.h"
#include "../Framework/Camera.h"
#include "../Framework/DeferredIllumination.h"
#include "SceneDataLoader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

#ifndef placingLight

#define placingLight placeState == PlaceState::LIGHT
#define placingRoom placeState == PlaceState::ROOM
#define placingObject placeState == PlaceState::OBJECT
#define placingCreature placeState == PlaceState::CREATURE

#endif // !placingLight


enum class PlaceState{
	OBJECT,
	LIGHT,
	ROOM,
	CREATURE
};

class EditorScene : public Scene
{
public:
	EditorScene(sf::RenderTarget *hwnd);
	~EditorScene() {};

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

	void setCurrentProp(const PhysicsObject &prop);

	void runPaletteWindow();
	void runColourPickerWindow();
	void runTextureWindow();

	void saveToFile(const std::string& filename);
	void loadFromFile(const std::string& filename);

	void loadProp(std::string const& filename);
	void saveProp(std::string const& filename, PhysicsObject propToBe);

private:
	struct PlacedObject
	{
		PhysicsObject obj;
		sf::String tex;
		bool selected = false;
		//int roomIndex = -1;
	};

	struct PlacedCreature : public PlacedObject
	{
		int roomIndex = -1; // -1 means not assigned to a room
	};

	PlaceState placeState = PlaceState::OBJECT;

	std::vector<PlacedObject> objects;
	int selectedIndex = -1;
	bool dragging = false;
	sf::Vector2f dragOffset;

	std::vector<PhysicsObject> paletteProps;
	PhysicsObject currentProp;

	void selectObjectAt(const sf::Vector2f &pos);
	void addObject(const sf::Vector2f &pos);
	void deleteSelectedObject();

	Camera camera;
	Commander commander;
	DeferredIllumination lighting;

	bool midLightPlace = false;
	std::vector<std::tuple<sf::Vector2f, float, sf::Color>> placedLights;

	int selectedLightIndex = -1;
	bool draggingLight = false;
	sf::Vector2f lightDragOffset;

	sf::Color currentColour = sf::Color::White;
	bool colourChanged = false;
	sf::String selectedTex;
	bool texChanged = false;
	float scrollOffset = 0.f;

	//rooms
	int activeRoomIndex = -1;
	std::vector<sf::FloatRect> placedRooms;

	sf::RenderWindow* paletteWin,* pickerWin, * texWin;
};
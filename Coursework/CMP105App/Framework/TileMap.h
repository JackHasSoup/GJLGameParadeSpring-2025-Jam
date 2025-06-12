#pragma once
#include <math.h>
#include "GameObject.h"

class TileMap
{
public:
	TileMap();
	~TileMap();

	//loads and stores the spritesheet containing all the tiles required to build the level/section
	void loadTexture(const char* filename);
	//receives an array of GameObjects representing the tile set (in order)
	void setTileSet(std::vector<GameObject> ts);
	//receives and array of integers and map dimensions representing the map (where and what tiles to place).
	void setTileMap(std::vector<int> tm, sf::Vector2u mapDimensions);
	//once provided with the map and tile set, builds the level, creating an array of tile sprites positioned based on the map. Ready to render.
	void buildLevel();

	//receives window handle and renders the level/tilemap
	void render(sf::RenderTarget* window);
	//returns the built level tile map. Used for collision detection, etc, where we need access to elements of the level.
	std::vector<GameObject>* getLevel(){ return &levelAssets; };

	//set the origin position of the tilemap section. 
	void setPosition(sf::Vector2f pos) { position = pos; };

protected:
	std::vector<GameObject> tileSet;
	std::vector<int> tileMap;
	std::vector<GameObject> levelAssets;
	sf::Texture texture;
	sf::Vector2u mapDimensions;
	sf::Vector2f position;
};


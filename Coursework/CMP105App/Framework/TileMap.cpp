#include "TileMap.h"

TileMap::TileMap()
{
	position = sf::Vector2f(0, 0);
}

TileMap::~TileMap()
{
}

void TileMap::render(sf::RenderTarget* window)
{
	for (int i = 0; i < (int)levelAssets.size(); i++)
	{
		window->draw(levelAssets[i]);
	}
}

void TileMap::loadTexture(const char* filename)
{
	texture.loadFromFile(filename);
}

void TileMap::setTileSet(std::vector<GameObject> ts)
{
	tileSet = ts;
}

void TileMap::setTileMap(std::vector<int> tm, sf::Vector2u _mapDimensions)
{
	tileMap = tm;
	mapDimensions = _mapDimensions;
}

void TileMap::buildLevel()
{
	if (tileSet.size() > 0 && tileMap.size() > 0)
	{
		int x, y = 0;
		sf::Vector2f tileSize(tileSet[0].getSize().x, tileSet[0].getSize().y);

		for (int i = 0; i < (int)tileMap.size(); i++)
		{
			x = i % mapDimensions.x;
			y = (int)floor(i / mapDimensions.x);
			tileSet[tileMap[i]].setPosition(position.x + (x * tileSize.x), position.y + (y * tileSize.y));
			levelAssets.push_back(tileSet[tileMap[i]]);
			levelAssets[i].setTexture(&texture);
		}
	}
}
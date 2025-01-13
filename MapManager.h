#pragma once
#include "Define.h"
#include "Vector2.h"

class MapManager{
public:
	MapManager();

	~MapManager() = default;

	void DrawTile(int posX, int posY, int tileNo, int graph);

	void DrawTileSet(float posX, float posY, Vector2 scrollPos, int(*map)[kMapWidth]);

private:
	int tileGraph_;

	
};




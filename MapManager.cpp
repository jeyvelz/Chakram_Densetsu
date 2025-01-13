#include "MapManager.h"
#include "Novice.h"

MapManager::MapManager() {
	tileGraph_ = Novice::LoadTexture("./Resources/block.png");
}

void MapManager::DrawTile(int posX, int posY, int tileNo, int graph) {
	Novice::DrawSpriteRect(posX, posY, tileNo * kBlockSize, tileNo * kBlockSize, kBlockSize, kBlockSize, graph, 1.0f, 1.0f, 0.0f, WHITE);
}

void MapManager::DrawTileSet(float posX, float posY, Vector2 scrollPos, int(*map)[kMapWidth]) {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x] == 1) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, tileGraph_);
			}
		}
	}
	Novice::ScreenPrintf(100, 100, "INGAMESCENE.CPP IS LOAD");
}

#include "MapManager.h"
#include "Novice.h"
#include "Object.h"

MapManager::MapManager() {
	tileGraph_ = Novice::LoadTexture("./Resources/tileset.png");

	// exit door
	exitGraph_ = Novice::LoadTexture("./Resources/exit.png");

	pos_ = { 137.0f, 958.0f };
	screenPos_ = pos_;
	widthHalf_ = 48.0f;
	heightHalf_ = 70.0f;

	// Key 
	keyGraph_ = Novice::LoadTexture("./Resources/key.png");

	keyPos_ = { 0.0f, 0.0f };
	keyScreenPos_ = pos_;
	keyWidthHalf_ = 20.0f;
	keyHeightHalf_ = 36.0f;

	isGetKey_ = false;

	// Tutorial
	tutorialMove = Novice::LoadTexture("./Resources/tutorial/tutorialMove.png");
	tutorialJump = Novice::LoadTexture("./Resources/tutorial/tutorialJump.png");
	tutorialAtk = Novice::LoadTexture("./Resources/tutorial/tutorialAtk.png");

	tutorialAim = Novice::LoadTexture("./Resources/tutorial/tutorialAim.png");
	animeFrame = 0;
	animePosX = 0;
	animePosY = 0;

	tutorialBackChakram = Novice::LoadTexture("./Resources/tutorial/tutorialBackChakram.png");;
	tutorialOnChakram = Novice::LoadTexture("./Resources/tutorial/tutorialOnChakram.png");;
}

void MapManager::DrawTile(int posX, int posY, int tileNoX, int tileNoY, int graph) {
	Novice::DrawSpriteRect(posX, posY, tileNoX * kBlockSize, tileNoY * kBlockSize, kBlockSize, kBlockSize, graph, 1.0f / 5.0f, 1.0f / 5.0f, 0.0f, WHITE);
}

void MapManager::DrawTileSet(float posX, float posY, Vector2 scrollPos, int(*map)[kMapWidth]) {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x] == 1) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, 0, tileGraph_);
			}
			if (map[y][x] == 2) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					1, 0, tileGraph_);
			}
			if (map[y][x] == 3) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					2, 0, tileGraph_);
			}
			if (map[y][x] == 4) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, 1, tileGraph_);
			}
			if (map[y][x] == 5) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					1, 1, tileGraph_);
			}
			if (map[y][x] == 6) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					2, 1, tileGraph_);
			}
			if (map[y][x] == 7) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, 2, tileGraph_);
			}
			if (map[y][x] == 8) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					1, 2, tileGraph_);
			}
			if (map[y][x] == 9) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					2, 2, tileGraph_);
			}
			if (map[y][x] == 10) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					3, 0, tileGraph_);
			}
			if (map[y][x] == 11) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					4, 0, tileGraph_);
			}
			if (map[y][x] == 12) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
				    3, 1, tileGraph_);
			}
			if (map[y][x] == 13) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					4, 1, tileGraph_);
			}
			if (map[y][x] == 14) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					3, 2, tileGraph_);
			}
			if (map[y][x] == 15) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					4, 2, tileGraph_);
			}
			if (map[y][x] == 16) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, 3, tileGraph_);
			}
			if (map[y][x] == 17) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					1, 3, tileGraph_);
			}
			if (map[y][x] == 18) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					2, 3, tileGraph_);
			}
			if (map[y][x] == 19) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					3, 3, tileGraph_);
			}
			if (map[y][x] == 20) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					4, 3, tileGraph_);
			}
			if (map[y][x] == 21) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					4, 4, tileGraph_);
			}
			if (map[y][x] == 30) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					0, 4, tileGraph_);
			}
			if (map[y][x] == 31) {
				DrawTile(static_cast<int>(posX - scrollPos.x) + (x * kBlockSize), static_cast<int>((posY + scrollPos.y) + (y * kBlockSize)),
					1, 4, tileGraph_);
			}
		}
	}
}

void MapManager::SetExitPos(float posX, float posY, Vector2 scrollPos) {
	pos_.x = posX;
	pos_.y = posY;

	screenPos_.x = pos_.x - scrollPos.x;
	screenPos_.y = pos_.y - scrollPos.y;

}

void MapManager::DrawExit() {
	DrawObject(screenPos_, widthHalf_, heightHalf_, 0, 0, exitGraph_, RIGHT, 0, 0);

	//Novice::ScreenPrintf(80, 80, "EXIT DRAWN");
}

void MapManager::SetKeyPos(float posX, float posY, Vector2 scrollPos) {
	keyPos_.x = posX;
	keyPos_.y = posY;

	keyScreenPos_.x = keyPos_.x - scrollPos.x;
	keyScreenPos_.y = keyPos_.y - scrollPos.y;

}

void MapManager::DrawKey() {
	if (isGetKey_ == false) {
		DrawObject(keyScreenPos_, keyWidthHalf_, keyHeightHalf_, 0, 0, keyGraph_, RIGHT, 0, 0);
	}

	//Novice::ScreenPrintf(80,100, "KEY %d", isGetKey_);
}

void MapManager::DrawTutorial(Vector2 scrollPos) {
	float pos0X = 240.0f - scrollPos.x;
	float pos0Y = 400.0f - scrollPos.y;
	DrawObject({ pos0X, pos0Y }, 100.0f, 100.0f, 0, 0, tutorialMove, RIGHT, 0, 0, WHITE);

	float pos1X = 500.0f - scrollPos.x;
	float pos1Y = 400.0f - scrollPos.y;
	DrawObject({ pos1X, pos1Y }, 100.0f, 100.0f, 0, 0, tutorialJump, RIGHT, 0, 0, WHITE);
	
	float pos2X = 1124.0f - scrollPos.x;
	float pos2Y = 446.0f - scrollPos.y;
	DrawObject({ pos2X, pos2Y }, 200.0f, 120.0f, 0, 0, tutorialAtk, RIGHT, 0, 0, WHITE);

	float pos3X = 1650.0f - scrollPos.x;
	float pos3Y = 446.0f - scrollPos.y;

	animeFrame++;
	animePosY = 0;
	if (animeFrame % 60 == 0) {
		animePosX += 400;

	}
	if (animePosX >= 400 * 5) {
		animePosX = 0;
		animeFrame = 0;
	}
	DrawObject({ pos3X, pos3Y }, 200.0f, 120.0f, animePosX, animePosY, tutorialAim, RIGHT, 0, 0, WHITE);

	float pos4X = 2050.0f - scrollPos.x;
	float pos4Y = 446.0f - scrollPos.y;
	DrawObject({ pos4X, pos4Y }, 120.0f, 120.0f, 0, 0, tutorialBackChakram, RIGHT, 0, 0, WHITE);

	float pos5X = 2404.0f - scrollPos.x;
	float pos5Y = 446.0f - scrollPos.y;
	DrawObject({ pos5X, pos5Y }, 160.0f, 120.0f, 0, 0, tutorialOnChakram, RIGHT, 0, 0, WHITE);
}
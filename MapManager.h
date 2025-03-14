#pragma once
#include "Define.h"
#include "Vector2.h"

class MapManager{
public:
	MapManager();

	~MapManager() = default;

	void DrawTile(int posX, int posY, int tileNoY, int tileNo, int graph);

	void DrawTileSet(float posX, float posY, Vector2 scrollPos, int(*map)[kMapWidth]);
	
	// Exit Function
	Vector2 GetExitPos() { return pos_; }
	float GetExitWidthHalf() { return widthHalf_; }
	float GetExitHeightHalf() { return heightHalf_; }

	void SetExitPos(float posX, float posY, Vector2 scrollPos);

	void DrawExit();

	// Key Function
	Vector2 GetKeyPos() { return keyPos_; }
	float GetKeyWidthHalf() { return keyWidthHalf_; }
	float GetKeyHeightHalf() { return keyHeightHalf_; }

	void SetKeyPos(float posX, float posY, Vector2 scrollPos);

	void DrawKey();

	// tutorials
	void DrawTutorial(Vector2 scrollPos);

private:
	int tileGraph_;
	
	// Exit Variables
	int exitGraph_;

	Vector2 pos_;
	Vector2 screenPos_;
	float widthHalf_;
	float heightHalf_;
	
	// Key Variables
	int keyGraph_;

	Vector2 keyPos_;
	Vector2 keyScreenPos_;
	float keyWidthHalf_;
	float keyHeightHalf_;

	// tutorials
	int tutorialMove;
	int tutorialJump;
	int tutorialAtk;

	int tutorialAim;
	int animeFrame;
	int animePosX;
	int animePosY;

	int tutorialBackChakram;
	int tutorialOnChakram;


public:
	bool isGetKey_;
};




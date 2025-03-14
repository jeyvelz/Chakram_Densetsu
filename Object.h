#pragma once
#include "Vertex.h"
#include <Novice.h>

enum DIRECTION {
	LEFT,
	RIGHT
};

class Object{
protected:
	// Position
	Vector2 spawnPos_;
	Vector2 pos_;
	Vector2 prePos_;
	Vector2 screenPos_;
	// Vertex
	Vector2 leftTop_;
	Vector2 rightTop_;
	Vector2 leftBottom_;
	Vector2 rightBottom_;

	Vector2 leftTopOld_;
	Vector2 rightTopOld_;
	Vector2 leftBottomOld_;
	Vector2 rightBottomOld_;
	
	// Speed
	Vector2 speed_;

	int hp_;

	// Size
	float widthHalf_;
	float heightHalf_;
	// ImageSize
	float drawWidthHalf_;
	float drawHeightHalf_;

	// Animation
	int animePosX_;
	int animePosY_;
	int animeFrame_;

	// shake
	int shakeX_;
	int shakeY_;

	// Graph
	int graph_;

public:

	DIRECTION direction_;

};

// DrawFunction
void DrawObject(Vector2 screenPos, float drawWidthHalf, float drawHeightHalf,
	int animePosX, int animePosY, int graph_, DIRECTION direction, int shakeX, int shakeY, unsigned int const color = WHITE);

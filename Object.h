#pragma once
#include "Vertex.h"


enum DIRECTION {
	LEFT,
	RIGHT
};

class Object{
protected:
	// Position
	Vector2 pos_;
	Vector2 prePos_;
	Vector2 screenPos_;
	Vector2 scrollPos_;
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
	// Size
	float widthHalf_;
	float heightHalf_;
	// ImageSize
	float drawWidthHalf_;
	float drawHeightHalf_;

	DIRECTION direction_;

	// Animation
	int animePosX_;
	int animePosY_;
	int animeFrame_;

	// Graph
	int graph_;

};


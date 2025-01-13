#pragma once
#include "Vector2.h"

struct Vertex {
	Vector2 leftTop;
	Vector2 leftBottom;
	Vector2 rightTop;
	Vector2 rightBottom;
};

struct TileVertex {
	Vector2Int leftTop;
	Vector2Int leftBottom;
	Vector2Int rightTop;
	Vector2Int rightBottom;
};

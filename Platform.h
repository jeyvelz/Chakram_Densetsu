#pragma once
#include "Object.h"

class Platform:public Object{
public:
	Platform();
	~Platform() = default;

	void Move();

	void Update(const Vector2 scroll);

	void Draw();

	void SpawnPlatform(Vector2 pos, float movementRange);

	Vector2 GetPos() { return pos_; }
	float GetPosY() { return pos_.y; }
	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

private:
	float movementRangeY_;
};


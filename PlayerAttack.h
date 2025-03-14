#pragma once
#include "Object.h"
#include "define.h"

class PlayerAttack :public Object {
public:
	PlayerAttack();
	~PlayerAttack() = default;

	void Attack(Vector2 const& pos, bool const& direction, Vector2 const& scroll);

	void Reset();

	float GetPosX() { return pos_.x; }
	float GetPosY() { return pos_.y; }

	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

	void DrawBoxCollision();

	bool isAtk_;
};


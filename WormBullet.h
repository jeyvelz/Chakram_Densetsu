#pragma once
#include "Vector2.h"
#include "Enemy.h"

class WormBullet:public Object{
public:
	WormBullet();
	~WormBullet() = default;

	void SpawnBullet(Vector2 const& pos, DIRECTION direction);
	void Update(const Vector2& scroll);

	void Draw();

	Vector2 GetPos() const { return pos_; }
	float GetPosX() const { return pos_.x; }
	float GetPosY() const { return pos_.y; }
	float GetWidthHalf() const { return widthHalf_; }
	float GetHeightHalf() const { return heightHalf_; }

private:
	Vector2 pos_;
	Vector2 speed_;
	Vector2 screenPos_;

	float widthHalf_;
	float heightHalf_;
	float drawWidthHalf_;
	float drawHeightHalf_;
	int graph_;

public:
	bool isShot_;
};


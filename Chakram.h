#pragma once
#include "Object.h"
#include "define.h"

enum CHAKRAM_STATE {
	NONE,
	PREAIM,
	AIM,
	LAUNCH,
	STUCK,
	SPINNING,
	RETURN,
};

class Chakram: public Object{
public:
	Chakram();
	~Chakram() = default;

	// void Aim(Vector2 const& playerPos);

	void AimControl(char* keys);
	// void Launch(char* keys, char* preKeys);

	void TargetMapCollision(int(*map)[kMapWidth]);

	void ChakramVertexUpdate();

	void TargetVertexUpdate();

	void Update(char* keys, char* preKeys, Vector2 const& playerPos, Vector2 const& scroll,
		DIRECTION const& playerDirection, int(*map)[kMapWidth]);

	void AnimeUpdate();

	void DrawChakram();

	void SetState(CHAKRAM_STATE state);

	int GetState() { return state_; }

	float GetPosX() { return pos_.x; }
	float GetPosY() { return pos_.y; }

	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }
	

private:
	// position
	Vector2 maxPos_;
	Vector2 stuckPos_;
	Vector2 launchPos_;

	Vector2 returnStartPos_;

	unsigned int color_;

	bool isPressLeft_;
	bool isPressRight_;
	bool isPressUp_;
	bool isPressDown_;

	bool canMoveLeft_;
	bool canMoveRight_;
	bool canMoveUp_;
	bool canMoveDown_;

	// timer
	float launchTimer_;
	float maxLaunchTimer_;

	int spinTimer_;
	int maxSpinTimer_;

	float returnTimer_;
	float maxReturnTimer_;

	CHAKRAM_STATE state_;

	bool isLaunched_;

	
	// target
	Vector2 targetPos_;
	Vector2 targetOldPos_;
	Vector2 targetScreenPos_;
	float targetRadius_;

	Vector2 maxAimDistance_;

	Vector2 aimSpeed_;

	Vector2 targetLeftTop_;
	Vector2 targetRightTop_;
	Vector2 targetLeftBottom_;
	Vector2 targetRightBottom_;

	Vector2 targetLeftTopOld_;
	Vector2 targetRightTopOld_;
	Vector2 targetLeftBottomOld_;
	Vector2 targetRightBottomOld_;

	// collision
	bool target_isPressLeft_;
	bool target_isPressRight_;
	bool target_isPressUp_;
	bool target_isPressDown_;

	bool target_canMoveLeft_;
	bool target_canMoveRight_;
	bool target_canMoveUp_;
	bool target_canMoveDown_;


	float targetWidthHalf_; // HitBox Size
	float targetHeightHalf_;
	float targetDrawWidthHalf_; // Image Size
	float targetDrawHeightHalf_;

	int crosshairGraph_;
};


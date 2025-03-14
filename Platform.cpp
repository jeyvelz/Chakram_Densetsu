#include "Platform.h"

Platform::Platform() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	speed_ = { 0.0f, 0.0f };

	widthHalf_ = 36.f;
	heightHalf_ = 16.f;

	drawWidthHalf_ = 36.f;
	drawHeightHalf_ = 16.f;

	movementRangeY_ = 120.0f;

	graph_ = Novice::LoadTexture("./Resources/platform.png");

}

void Platform::SpawnPlatform(Vector2 pos, float movementRange) {
	spawnPos_ = pos;
	pos_ = spawnPos_;
	movementRangeY_ = movementRange;
}

void Platform::Move() {
	pos_.x += speed_.x;
	if (pos_.x >= spawnPos_.x + movementRangeY_ || pos_.x <= spawnPos_.x - movementRangeY_) {
		speed_.x = -speed_.x;
	}
}

void Platform::Update(const Vector2 scroll) {
	Move();
	screenPos_.x = pos_.x - scroll.x;
	screenPos_.y = pos_.y - scroll.y;
	
}

void Platform::Draw() {
	DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, 0, 0, graph_, RIGHT, 0, 0);

	Novice::ScreenPrintf(0, 100, "PLATFORM IS DRAWN");
}

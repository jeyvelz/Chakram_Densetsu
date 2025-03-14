#include "WormBullet.h"
#include <Novice.h>
#include <math.h>

WormBullet::WormBullet() {
	pos_ = { -999.0f, -999.0f };

	screenPos_ = pos_;
	speed_ = { 4.0f, 0.0f };

	widthHalf_ = 10.0f;
	heightHalf_ = 10.0f;
	drawWidthHalf_ = 10.0f;
	drawHeightHalf_ = 10.0f;

	isShot_ = false;

	graph_ = Novice::LoadTexture("./Resources/enemy/worm_bullet.png");
}

void WormBullet::SpawnBullet(Vector2 const& pos, DIRECTION direction) {
	
	isShot_ = true;
	pos_ = pos;
	pos_.y = pos.y + 36.0f;
	if (direction == RIGHT) {
		pos_.x = pos.x + 20.0f;
		speed_.x = 4.0f;
	} else {
		pos_.x = pos.x - 20.0f;
		speed_.x = -4.0f;
	}
	
}

void WormBullet::Update(const Vector2& scroll) {
	if (isShot_ == true) {
		pos_.x += speed_.x;
		pos_.y += speed_.y;
		screenPos_.x = pos_.x - scroll.x;
		screenPos_.y = pos_.y - scroll.y;
	}
}

void WormBullet::Draw() {
	if (isShot_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, 0, 0, graph_, direction_, 0, 0);
	}
}

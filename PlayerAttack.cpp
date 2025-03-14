#include "PlayerAttack.h"
#include "ToScreen.h"

PlayerAttack::PlayerAttack() {
	pos_.x = -100.f;
	pos_.y = -100.f;
	widthHalf_ = 32.f;
	heightHalf_ = 62.f;

	isAtk_ = false;
}

void PlayerAttack::Attack(Vector2 const& pos, bool const& direction, Vector2 const& scroll) {
	pos_.y = pos.y;
	if (direction) {
		pos_.x = pos.x + widthHalf_;
	} else {
		pos_.x = pos.x - widthHalf_;
	}

	screenPos_.x = pos_.x - scroll.x;
	screenPos_.y = pos_.y - scroll.y;
}


void PlayerAttack::Reset() {
	pos_.x = -100.f;
	pos_.y = -100.f;
}

void PlayerAttack::DrawBoxCollision() {
	if (isAtk_ == true) {
		Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);
	}
}
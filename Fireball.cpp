#include "Fireball.h"
#include "ToScreen.h"

const float kFireballSpeed = 8.0f;
const int kImageWidth = 80;

Fireball::Fireball() {
	for (int i = 0; i < kMaxFireball; i++) {
		fireballs[i].pos_ = { 0.0f, 0.0f };
		fireballs[i].screenPos_ = { 0.0f, 0.0f };
		fireballs[i].widthHalf_ = 20.0f;
		fireballs[i].heightHalf_ = 14.0f;
		fireballs[i].speedX_ = 0.0f;
		fireballs[i].direction_ = RIGHT;
		fireballs[i].isVisible_ = false;
	};

	drawWidthHalf_ = 40.0f;
	drawHeightHalf_ = 16.0f;

	timer_ = 0;
	isLaunched_ = false;
	cooldown_ = 300;
	
	animePosX_ = 0;
	animePosY_ = 0;
	animeFrame_ = 0;

	graph_ = Novice::LoadTexture("./Resources/boss/fireball.png");
}

void Fireball::Reset() {
	for (int i = 0; i < kMaxFireball; i++) {
		fireballs[i].pos_ = { 0.0f, 0.0f };
		fireballs[i].screenPos_ = { 0.0f, 0.0f };
		fireballs[i].widthHalf_ = 20.0f;
		fireballs[i].heightHalf_ = 14.0f;
		fireballs[i].speedX_ = 0.0f;
		fireballs[i].direction_ = RIGHT;
		fireballs[i].isVisible_ = false;
	};

	timer_ = 0;
	isLaunched_ = false;
	cooldown_ = 300;

}

bool Fireball::Collision(const Vector2 pos, float widthHalf, float heightHalf) {
	for (int i = 0; i < kMaxFireball; i++) {
		if (fireballs[i].isVisible_ == true) {
			if (fireballs[i].pos_.x - fireballs[i].widthHalf_< pos.x + widthHalf && fireballs[i].pos_.x + fireballs[i].widthHalf_ > pos.x - widthHalf) {
				if (fireballs[i].pos_.y - fireballs[i].heightHalf_ < pos.y + heightHalf && fireballs[i].pos_.y + fireballs[i].heightHalf_ > pos.y - heightHalf) {

					return true;
				}
			}
		}
	}

	return false;
}

void Fireball::Update(const Vector2 scrollPos) {
	if (isLaunched_) {
		timer_++;
		if (timer_ % 40 == 0) {
			for (int i = 0; i < kMaxFireball; i++) {
				if (fireballs[i].isVisible_ == false){
					fireballs[i].pos_.y = 200.0f;
					fireballs[i].isVisible_ = true;
					if (i % 2 == 0) {
						fireballs[i].pos_.x = 0.0f;
						fireballs[i].speedX_ = kFireballSpeed;
						fireballs[i].direction_ = RIGHT;
					} else {
						fireballs[i].pos_.x = 2600.0f;
						fireballs[i].speedX_ = -kFireballSpeed;
						fireballs[i].direction_ = LEFT;
					}

					break;
				}
			}
		}
	}

	for (int i = 0; i < kMaxFireball; i++) {
		if (fireballs[i].isVisible_ == true) {
			fireballs[i].pos_.x += fireballs[i].speedX_;

			if (fireballs[i].direction_ == RIGHT) {
				if (fireballs[i].pos_.x >= 2600.0f) {
					fireballs[i].speedX_ = 0.0f;
					fireballs[i].isVisible_ = false;
				} 
			} else {
				if (fireballs[i].pos_.x <= 10.0f) {
					fireballs[i].speedX_ = 0.0f;
					fireballs[i].isVisible_ = false;
				}
			}

			fireballs[i].screenPos_.x = fireballs[i].pos_.x - scrollPos.x;
			fireballs[i].screenPos_.y = fireballs[i].pos_.y - scrollPos.y;

			if (i >= kMaxFireball - 1) {
				isLaunched_ = false;
				
				break;
			}
		}
	}

	animeFrame_++;
	animePosY_ = 0;
	if (animeFrame_ % 6 == 0) {
		animePosX_ += kImageWidth;
	}
	if (animePosX_ >= kImageWidth * 3) {
		animePosX_ = 0;
		animeFrame_ = 0;
	}
}

void Fireball::Draw() {
	for (int i = 0; i < kMaxFireball; i++) {
		if (fireballs[i].isVisible_ == true) {
			DrawObject(fireballs[i].screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, fireballs[i].direction_, 0, 0);

			/*Novice::DrawBox(static_cast<int>(fireballs[i].screenPos_.x - fireballs[i].widthHalf_), static_cast<int>(ConvertToScreen(fireballs[i].screenPos_.y) + fireballs[i].heightHalf_),
			static_cast<int>(fireballs[i].widthHalf_ * 2.f), static_cast<int>(-(fireballs[i].heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

			//Novice::ScreenPrintf(920, 100 + i * 20, "FB X %0.2f Y %0.2f", fireballs[i].pos_.x, fireballs[i].pos_.y);
		}
	}

	/*Novice::ScreenPrintf(0, 80, "isLaunched_ %d", isLaunched_);
	Novice::ScreenPrintf(0, 100, "timer %d CD %d", timer_, cooldown_);*/
}

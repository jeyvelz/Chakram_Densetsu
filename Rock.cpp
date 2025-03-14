#include "Rock.h"
#include "Novice.h"

Rock::Rock() {
	for (int i = 0; i < kMaxRocks; i++) {
		rocks[i].pos_ = { 0.0f, 0.0f };
		rocks[i].screenPos_ = { 0.0f, 0.0f };
		rocks[i].widthHalf_ = 64.0f;
		rocks[i].heightHalf_ = 64.0f;
		rocks[i].speedY_ = 0.0f;
		rocks[i].direction_ = RIGHT;
		rocks[i].isVisible_ = false;
	};

	timer_ = 0;

	drawWidthHalf_ = 64.0f;
	drawHeightHalf_ = 64.0f;

	spawnWidth_ = 800;
	spawnHeight_ = 100;
	particleCount_ = 0;

	isLaunched_ = false;

	graph_ = Novice::LoadTexture("./Resources/boss/rock.png");

	// warning

	warningPos = { 1600.0f, 128.0f + 580.0f};
	warningScreenPos = warningPos;
	
	warningGraph_ = Novice::LoadTexture("./Resources/boss/caution.png");
}

bool Rock::Collision(const Vector2 pos, float widthHalf, float heightHalf) {
	for (int i = 0; i < kMaxRocks; i++) {
		if (rocks[i].isVisible_ == true) {
			if (rocks[i].pos_.x - rocks[i].widthHalf_< pos.x + widthHalf && rocks[i].pos_.x + rocks[i].widthHalf_ > pos.x - widthHalf) {
				if (rocks[i].pos_.y - rocks[i].heightHalf_ < pos.y + heightHalf && rocks[i].pos_.y + rocks[i].heightHalf_ > pos.y - heightHalf) {

					return true;
				}
			}
		}
	}

	return false;
}

void Rock::Update(const Vector2 scroll) {
	if (isLaunched_) {
		timer_++;
		if (timer_ % 4 == 0) {
			for (int i = 0; i < kMaxRocks; i++) {
				/// パーティクルのスポーン処理
				if (rocks[i].isVisible_ == false) {
					rocks[i].isVisible_ = true;
					
					rocks[i].pos_.x = static_cast<float>((rand() % spawnWidth_ - spawnWidth_ / 2) + 1600);
					rocks[i].pos_.y = static_cast<float>((rand() % spawnHeight_ - spawnHeight_ / 2) + 1600);

					rocks[i].widthHalf_ = 64.0f;
					rocks[i].heightHalf_ = 64.0f;

					if (i % 2 == 0) {
						rocks[i].direction_ = RIGHT;
					} else {
						rocks[i].direction_ = LEFT;
					}

					particleCount_++;
					if (particleCount_ >= 1) {
						particleCount_ = 0;
						break;
					}
				}
			}
		}
	}

	/// パーティクルの移動処理
	for (int i = 0; i < kMaxRocks; i++) {
		if (rocks[i].isVisible_ == true) {
			rocks[i].speedY_ -= 0.08f;
			rocks[i].pos_.y += rocks[i].speedY_;

			/// パーティクルをリセット
			if (rocks[i].pos_.y - rocks[i].heightHalf_ <= -100.0f) {
				rocks[i].isVisible_ = false;
				rocks[i].speedY_ = 0;
			}

			rocks[i].screenPos_.x = rocks[i].pos_.x - scroll.x;
			rocks[i].screenPos_.y = rocks[i].pos_.y - scroll.y;

			if (i >= kMaxRocks - 1) {
				isLaunched_ = false;
				break;
			}
		}
	}

	warningPos = { 1600.0f, 1056.0f };
	warningScreenPos.x = warningPos.x - scroll.x;
	warningScreenPos.y = 1368.0f;;

}

void Rock::Draw() {
	for (int i = 0; i < kMaxRocks; i++) {
		if (rocks[i].isVisible_ == true) {
			DrawObject(rocks[i].screenPos_, drawWidthHalf_, drawHeightHalf_, 0, 0, graph_, rocks[i].direction_, 0, 0);
			
		}
	}
	if (isLaunched_) {
		DrawObject(warningScreenPos, 448, 32, 0, 0, warningGraph_, RIGHT, 0, 0);
		//DrawObject(warningScreenPos2, 448, 32, 0, 0, warningGraph_, RIGHT, 0, 0);
	}

	/*Novice::ScreenPrintf(0, 80, "isLaunched_ %d", isLaunched_);
	Novice::ScreenPrintf(0, 100, "timer_ %d", timer_);*/
}


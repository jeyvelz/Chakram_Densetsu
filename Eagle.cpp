#include "Eagle.h"
#include "Novice.h"
#include "ToScreen.h"

const int maxHp_ = 3;
const int maxInvincibleTimer_ = 20;


Eagle::Eagle() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	speed_ = { 2.1f, 0.0f };

	widthHalf_ = 56.f;
	heightHalf_ = 18.f;

	drawWidthHalf_ = 72.f;
	drawHeightHalf_ = 48.f;

	imageWidth_ = 144;
	imageHeight_ = 96;
	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;

	direction_ = RIGHT;
	state_ = ENEMY_IDLE;

	hp_ = maxHp_;
	isSpawn_ = false;

	movementRangeY_ = 120.0f;

	graph_ = Novice::LoadTexture("./Resources/enemy/eagle.png");
}

void Eagle ::SetPos(Vector2 pos) {
	spawnPos_ = pos;
}

void Eagle::SetState(ENEMY_STATE state) {
	state_ = state;
}

void Eagle::SpawnEagle(Vector2 pos, float movementRange) {
	spawnPos_ = pos;
	pos_ = spawnPos_;
	movementRangeY_ = movementRange;

	hp_ = maxHp_;
	isSpawn_ = true;
	state_ = ENEMY_IDLE;

	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;
}

void Eagle::Move() {
	if (state_ == ENEMY_IDLE) {
		pos_.x += speed_.x;
		if (pos_.x >= spawnPos_.x + movementRangeY_ || pos_.x <= spawnPos_.x - movementRangeY_) {
			speed_.x = -speed_.x;
		}
		if (speed_.x >= 0.0f) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}
	}
}

void Eagle::Update(const Player* player) {
	if (player == nullptr) {
		return;
	}

	if (isSpawn_ == true) {
		Move();
		AnimeUpdate();
		screenPos_.x = pos_.x - player->GetScrollPosX();
		screenPos_.y = pos_.y - player->GetScrollPosY();
	}

	if (hp_ <= 0) {
		state_ = ENEMY_DEAD;
		pos_.y -= 2.2f;
	}
}

bool Eagle::WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();

	if (isInvincible_ == false) {
		if (hp_ > 0) {
			if (enemyPos.x - enemyWidthHalf < chakramPosX + chakramWidthHalf && enemyPos.x + enemyWidthHalf > chakramPosX - chakramWidthHalf) {
				if (enemyPos.y - enemyHeightHalf < chakramPosY + chakramHeightHalf && enemyPos.y + enemyHeightHalf > chakramPosY - chakramHeightHalf) {
					isInvincible_ = true;
					color_ = RED;
					if (hp_ <= 0) {
						animeFrame_ = 0;
						animePosX_ = 0;
					}
					return true;
				}
			}
		}
	} else {
		invincibleTimer_++;
		if (invincibleTimer_ > 10) {
			color_ = WHITE;
		}
		if (invincibleTimer_ > maxInvincibleTimer_) {
			invincibleTimer_ = 0;
			isInvincible_ = false;
		}
	}

	return false;
}

void Eagle::GiveDamage(int damage) {
	hp_ -= damage;
}

void Eagle::AnimeUpdate() {
	animeFrame_++;
	switch (state_) {
	case(ENEMY_IDLE):
		animePosY_ = 0;
		if (animeFrame_ % 8 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 4) {
			animePosX_ = 0;
			animeFrame_ = 0;
		}
		break;
	case(ENEMY_DEAD):
		animePosY_ = imageHeight_;
		if (animeFrame_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 2) {
			animePosX_ = imageWidth_;
		}
		if (animeFrame_ > 60) {
			isSpawn_ = false;
		}
		break;
	}
}

void Eagle::DrawEnemy() {
	if (isSpawn_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, direction_, 0, 0, color_);

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

	}
	// Novice::ScreenPrintf(200, 400, "animeposX %d anime frame %d", animePosX_, animeFrame_);
}
#include "FireSpirit.h"
#include "Novice.h"
#include "ToScreen.h"

#include <math.h>

const int maxHp_ = 3;

FireSpirit::FireSpirit() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	speed_ = { 0.0f, 0.0f };

	widthHalf_ = 10.f;
	heightHalf_ = 10.f;

	attackDistance_ = 400.0f;

	drawWidthHalf_ = 20.f;
	drawHeightHalf_ = 24.f;

	imageWidth_ = 40;
	imageHeight_ = 48;

	deadFrame_ = 0;
	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;

	direction_ = RIGHT;
	state_ = ENEMY_IDLE;

	hp_ = maxHp_;
	isSpawn_ = false;

	maxInvincibleTimer_ = 20;

	graph_ = Novice::LoadTexture("./Resources/enemy/firespirit.png");
}

void FireSpirit::SetPos(Vector2 pos) {
	spawnPos_ = pos;
}

void FireSpirit::SetState(ENEMY_STATE state) {
	state_ = state;
}

void FireSpirit::SpawnFireSpirit(Vector2 pos, float attackDistance) {
	spawnPos_ = pos;
	pos_ = spawnPos_;
	speed_ = { 0.0f, 0.0f };

	attackDistance_ = attackDistance;

	hp_ = maxHp_;
	isSpawn_ = true;
	state_ = ENEMY_IDLE;
	
	deadFrame_ = 0;
	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;
}

void FireSpirit::Move(const Player* player) {
	if (player == nullptr) {
		return;
	}

	if (state_ != ENEMY_DEAD) {
		if (pos_.x <= player->GetPosX()) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}

		/// Check Player pos to attack
		Vector2 enemyToPlayer = { player->GetPosX() - pos_.x, player->GetPosY() - pos_.y };
		float distanceToPlayer = sqrtf(enemyToPlayer.x * enemyToPlayer.x + enemyToPlayer.y * enemyToPlayer.y);

		if (distanceToPlayer <= attackDistance_) {
			Vector2 atkSpeed = { 2.6f, 1.2f };

			if (player->GetPosX() >= pos_.x) {
				speed_.x = atkSpeed.x;
			} else {
				speed_.x = -atkSpeed.x;
			}


			if (player->GetPosY() >= pos_.y + 4.0f) {
				speed_.y = atkSpeed.y;
			} else if (player->GetPosY() <= pos_.y - 4.0f) {
				speed_.y = -atkSpeed.y;
			} else {
				speed_.y = 0.0f;
			}
		} else if (distanceToPlayer >= 720.0f) {
			speed_ = { 0.0f, 0.0f };
		}

		pos_.x += speed_.x;
		pos_.y += speed_.y;
	}
}

bool FireSpirit::WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) {
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

bool FireSpirit::SlashCollision(PlayerAttack* slash) {
	float slashPosX = slash->GetPosX();
	float slashPosY = slash->GetPosY();

	float slashWidthHalf = slash->GetWidthHalf();
	float slashHeightHalf = slash->GetHeightHalf();

	if (isInvincible_ == false && state_ != ENEMY_DEAD) {
		if (hp_ > 0) {
			if (pos_.x - widthHalf_ < slashPosX + slashWidthHalf && pos_.x + widthHalf_ > slashPosX - slashWidthHalf) {
				if (pos_.y - heightHalf_ < slashPosY + slashHeightHalf && pos_.y + heightHalf_ > slashPosY - slashHeightHalf) {
					isInvincible_ = true;
					color_ = RED;
					if (hp_ <= 0) {
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


void FireSpirit::Update(const Player* player) {
	if (player == nullptr) {
		return;
	}

	if (isSpawn_ == true) {
		Move(player);
		AnimeUpdate();
		screenPos_.x = pos_.x - player->GetScrollPosX();
		screenPos_.y = pos_.y - player->GetScrollPosY();
	}

	if (hp_ <= 0) {
		state_ = ENEMY_DEAD;
		
	}
}

void FireSpirit::GiveDamage(int damage) {
	hp_ -= damage;
}

void FireSpirit::AnimeUpdate() {

	if (state_ == ENEMY_DEAD) {
		deadFrame_++;
		animePosY_ = imageHeight_;
		if (deadFrame_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 4) {
			animePosX_ = 0;
			isSpawn_ = false;
		}
		
	} else {
		animeFrame_++;
		animePosY_ = 0;
		if (animeFrame_ % 8 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 4) {
			animePosX_ = 0;
		}
	}
}

void FireSpirit::DrawEnemy() {
	if (isSpawn_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, direction_, 0, 0, color_);

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

	}
	// Novice::ScreenPrintf(200, 400, "animeposX %d anime frame %d", animePosX_, animeFrame_);
}
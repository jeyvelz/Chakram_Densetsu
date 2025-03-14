#include "Worm.h"
#include "Novice.h"
#include "ToScreen.h"
#include "ToTile.h"

const int maxHp_ = 5;

Worm::Worm() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	speed_ = { 0.0f, 0.0f };

	widthHalf_ = 24.f;
	heightHalf_ = 48.f;

	drawWidthHalf_ = 52.f;
	drawHeightHalf_ = 68.f;

	imageWidth_ = 104;
	imageHeight_ = 136;
	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;

	direction_ = RIGHT;
	state_ = ENEMY_IDLE;

	isSpawn_ = false;
	hp_ = maxHp_;

	maxInvincibleTimer_ = 25;
	atkTimer_ = 0;
	atkTiming_ = 180;

	// vertex
	leftTop_.x = pos_.x - widthHalf_;
	leftTop_.y = pos_.y + heightHalf_;
	rightTop_.x = pos_.x + widthHalf_ - 1.0f;
	rightTop_.y = pos_.y + heightHalf_;
	leftBottom_.x = pos_.x - widthHalf_;
	leftBottom_.y = pos_.y - heightHalf_ + 1.0f;
	rightBottom_.x = pos_.x + widthHalf_ - 1.0f;
	rightBottom_.y = pos_.y - heightHalf_ + 1.0f;

	leftTopOld_.x = prePos_.x - widthHalf_;
	leftTopOld_.y = prePos_.y + heightHalf_;
	rightTopOld_.x = prePos_.x + widthHalf_ - 1.0f;
	rightTopOld_.y = prePos_.y + heightHalf_;
	leftBottomOld_.x = prePos_.x - widthHalf_;
	leftBottomOld_.y = prePos_.y - heightHalf_ + 1.0f;
	rightBottomOld_.x = prePos_.x + widthHalf_ - 1.0f;
	rightBottomOld_.y = prePos_.y - heightHalf_ + 1.0f;
	
	bullet_ = new WormBullet;

	graph_ = Novice::LoadTexture("./Resources/enemy/worm.png");
}

void Worm::SetPos(Vector2 pos) {
	spawnPos_ = pos;
}

void Worm::SetState(ENEMY_STATE state) {
	state_ = state;
}

void Worm::Attack() {
	if (state_ != ENEMY_DEAD) {
		atkTimer_++;
		if (atkTimer_ == atkTiming_) {
			state_ = ENEMY_ATTACK;
		}
		if (atkTimer_ == atkTiming_ + 30) { // 30 = 1 atk anime frame
			bullet_->SpawnBullet(pos_, direction_);
		}
		if (atkTimer_ == atkTiming_ + 60) {
			atkTimer_ = 0;
			state_ = ENEMY_IDLE;
		}
	}
}

void Worm::Update(const Player* player) {
	if (player == nullptr) {
		return;
	}

	bullet_->Update(player->GetScrollPos());
	if (isSpawn_ == true) {
		if (pos_.x <= player->GetPosX()) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}

		Attack();
		
		AnimeUpdate();
		screenPos_.x = pos_.x - player->GetScrollPosX();
		screenPos_.y = pos_.y - player->GetScrollPosY();
	}

	if (hp_ <= 0) {
		state_ = ENEMY_DEAD;
	}
}

void Worm::SpawnWorm(Vector2 pos) {
	spawnPos_ = pos;
	pos_ = spawnPos_;
	hp_ = maxHp_;
	isSpawn_ = true;
	state_ = ENEMY_IDLE;

	bullet_->isShot_ = false;

	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;

	atkTimer_ = 0;
}

void Worm::GiveDamage(int damage) {
	hp_ -= damage;
}

bool Worm::WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();

	if (isInvincible_ == false && state_ != ENEMY_DEAD) {
		if (hp_ > 0) {
			if (enemyPos.x - enemyWidthHalf < chakramPosX + chakramWidthHalf && enemyPos.x + enemyWidthHalf > chakramPosX - chakramWidthHalf) {
				if (enemyPos.y - enemyHeightHalf < chakramPosY + chakramHeightHalf && enemyPos.y + enemyHeightHalf > chakramPosY - chakramHeightHalf) {
					isInvincible_ = true;
					color_ = RED;
					if (hp_ <= 0) {
						animeFrame_ = 0;
						animePosX_ = 0;

						state_ = ENEMY_DEAD;
					}
					return true;
				}
			}
		}
	} else {
		invincibleTimer_++;
		if (invincibleTimer_ > 10) {
			if (hp_ > 0) {
				state_ = ENEMY_IDLE;
			}
			color_ = WHITE;
		}
		if (invincibleTimer_ > maxInvincibleTimer_) {
			invincibleTimer_ = 0;
			isInvincible_ = false;
		}
	}

	return false;
}

bool Worm::SlashCollision(PlayerAttack* slash) {
	float slashPosX = slash->GetPosX();
	float slashPosY = slash->GetPosY();

	float slashWidthHalf = slash->GetWidthHalf();
	float slashHeightHalf = slash->GetHeightHalf();

	if (isInvincible_ == false && state_ != ENEMY_DAMAGED) {
		if (hp_ > 0) {
			if (pos_.x - widthHalf_ < slashPosX + slashWidthHalf && pos_.x + widthHalf_ > slashPosX - slashWidthHalf) {
				if (pos_.y - heightHalf_ < slashPosY + slashHeightHalf && pos_.y + heightHalf_ > slashPosY - slashHeightHalf) {
					isInvincible_ = true;
					color_ = RED;
					state_ = ENEMY_DAMAGED;
					if (hp_ <= 0) {
						animeFrame_ = 0;
						animePosX_ = 0;
						state_ = ENEMY_DEAD;
					}
					maxInvincibleTimer_ = 30;
					return true;
				}
			}
		}
	} else {
		invincibleTimer_++;
		if (invincibleTimer_ > 10) {
			if (hp_ > 0) {
				state_ = ENEMY_IDLE;
			}
			color_ = WHITE;
		}
		if (invincibleTimer_ > maxInvincibleTimer_) {
			invincibleTimer_ = 0;
			isInvincible_ = false;

		}
	}

	return false;
}

void Worm::AnimeUpdate() {
	animeFrame_++;
	switch (state_) {
	case(ENEMY_IDLE):
			animePosY_ = 0;
			if (animeFrame_ % 8 == 0) {
				animePosX_ += imageWidth_;

			}
			if (animePosX_ >= imageWidth_ * 3) {
				animePosX_ = 0;
			}
		break;
	case(ENEMY_ATTACK):
		animePosY_ = imageHeight_;
		if (animeFrame_ % 30 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 2) {
			animePosX_ = 0;
		}
		break;
	case(ENEMY_DEAD):
		animePosY_ = imageHeight_ * 2;
		if (animeFrame_ % 8 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 4) {
			animePosX_ = 0;
			isSpawn_ = false;
		}
		break;
	}
}

void Worm::DrawEnemy() {
	if (isSpawn_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, 
			graph_, direction_, 0, 0, color_);

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/
	}

	bullet_->Draw();
}
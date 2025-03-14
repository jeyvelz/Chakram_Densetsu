#include "Slime.h"
#include "Novice.h"
#include "ToScreen.h"
#include "ToTile.h"

const int maxHp_ = 4;

Slime::Slime() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	speed_ = { 1.0f, 0.0f };

	widthHalf_ = 30.f;
	heightHalf_ = 30.f;

	drawWidthHalf_ = 52.f;
	drawHeightHalf_ = 52.f;

	imageWidth_ = 104;
	imageHeight_ = 104;
	animeFrame_ = 0;
	animePosX_ = 0;
	animePosY_ = 0;

	isGround_ = false;
	direction_ = RIGHT;
	state_ = ENEMY_IDLE;

	knockback_ = { 10.2f, 14.0f };

	maxInvincibleTimer_ = 20;
	hp_ = maxHp_;
	isSpawn_ = false;

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

	graph_ = Novice::LoadTexture("./Resources/enemy/slime.png");
}

void Slime::SetPos(Vector2 pos) {
	spawnPos_ = pos;
}

void Slime::SetState(ENEMY_STATE state) {
	state_ = state;
}

void Slime::MapCollision(int(*map)[kMapWidth]){
	isGround_ = false;
	prePos_.y = pos_.y;
	speed_.y += kGravity;

	if (map[BlockY(leftBottom_.y + speed_.y - 1.0f)][BlockX(leftBottom_.x)] >= 1 &&
		map[BlockY(rightBottom_.y + speed_.y - 1.0f)][BlockX(rightBottom_.x)] >= 1) {
		isGround_ = true;
		speed_.y = 0.0f;
		while (true) {
			pos_.y++;
			VertexUpdate();
			if (map[BlockY(leftBottom_.y + speed_.y - 1.0f)][BlockX(leftBottom_.x)] == 0 &&
				map[BlockY(rightBottom_.y + speed_.y - 1.0f)][BlockX(rightBottom_.x)] == 0) {
				pos_.y--;
				break;
			}
		}

	} else {
		pos_.y += speed_.y;
		//speed_.x = -speed_.x;
		if (map[BlockY(leftBottom_.y + speed_.y - 1.0f)][BlockX(leftBottom_.x)] != 0 &&
			map[BlockY(rightBottom_.y + speed_.y - 1.0f)][BlockX(rightBottom_.x)] != 0) {
			speed_.x *= -1.f;
		}
	}
	
	
	// Left
	if (map[BlockY(leftTop_.y - 1.0f)][BlockX(leftTop_.x + speed_.x)] >= 1 ||
		map[BlockY(leftBottom_.y)][BlockX(leftBottom_.x + speed_.x)] >= 1 ||
		map[BlockY(leftBottom_.y) + 1][BlockX(leftBottom_.x + speed_.x)] == 0) {
		speed_.x = -speed_.x;
	}
	// Right
	if (map[BlockY(rightTop_.y - 1.0f)][BlockX(rightTop_.x + speed_.x)] >= 1 ||
		map[BlockY(rightBottom_.y)][BlockX(rightBottom_.x + speed_.x)] >= 1 || 
		map[BlockY(rightBottom_.y) + 1][BlockX(rightBottom_.x + speed_.x)] == 0 ) {
		speed_.x = -speed_.x;
	}
}

void Slime::VertexUpdate() {
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
}

void Slime::Move() {
	if (state_ != ENEMY_DEAD && isGround_ == true) {
		pos_.x += speed_.x;

		if (speed_.x >= 0.0f) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}
	}
}

void Slime::Update(const Player* player, int(*map)[kMapWidth], Vector2 slashPos) {
	if (player == nullptr) {
		return;
	}
	if (isSpawn_ == true) {
		Move();
		Damaged(slashPos);
		VertexUpdate();
		MapCollision(map);
		AnimeUpdate();
		screenPos_.x = pos_.x - player->GetScrollPosX();
		screenPos_.y = pos_.y - player->GetScrollPosY();
	}


	if (hp_ <= 0) {
		state_ = ENEMY_DEAD;
	}
}

void Slime::SpawnSlime(Vector2 pos) {
	spawnPos_ = pos;
	pos_ = spawnPos_;
	hp_ = maxHp_;
	isSpawn_ = true;
	state_ = ENEMY_IDLE;
}

void Slime::GiveDamage(int damage) {
	hp_ -= damage;
}

void Slime::Damaged(Vector2 slashPos) {
	if (state_ == ENEMY_DAMAGED) {
		
		if (slashPos.x <= pos_.x) {
		}
	}
}

bool Slime::WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();

	if (isInvincible_ == false && state_ != ENEMY_DEAD && chakram->GetState() >= LAUNCH) {
		if (hp_ > 0) {
			if (enemyPos.x - enemyWidthHalf < chakramPosX + chakramWidthHalf && enemyPos.x + enemyWidthHalf > chakramPosX - chakramWidthHalf) {
				if (enemyPos.y - enemyHeightHalf < chakramPosY + chakramHeightHalf && enemyPos.y + enemyHeightHalf > chakramPosY - chakramHeightHalf) {
					isInvincible_ = true;
					color_ = RED;
					if (hp_ <= 0) {
						animeFrame_ = 0;
						animePosX_ = 0;
					}
					maxInvincibleTimer_ = 20;
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

bool Slime::SlashCollision(PlayerAttack* slash) {
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
					}
					maxInvincibleTimer_ = 40;
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

void Slime::AnimeUpdate() {
	animeFrame_++;
	switch (state_) {
	case(ENEMY_IDLE):
		animePosY_ = 0;
		if (animeFrame_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 3) {
			animePosX_ = 0;
		}
		break;
	case(ENEMY_DEAD):
		animePosY_ = imageHeight_;
		if (animeFrame_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * 3) {
			animePosX_ = 0;
			isSpawn_ = false;
		}
		break;
	}
}

void Slime::DrawEnemy() {
	if (isSpawn_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_,
			graph_, direction_, 0, 0, color_);

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

	}

	/*Novice::ScreenPrintf(100, 200, "slime HP %d", hp_);
	Novice::ScreenPrintf(100, 220, "maxInvin %d", maxInvincibleTimer_);*/
}


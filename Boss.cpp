#include "Boss.h"
#include "ToScreen.h"

const float kGroundPos_ = 128.0f;
const float kStompSpeedUp = 2.0f;
const float kStompSpeedDown = -18.0f;
const float kFootWidthHalf = 100.0f;
const float kFootHeightHalf = 30.0f;
const int maxHP = 100; // 100
const int footHP = 30; // 30

Boss::Boss() {
	hp_ = maxHP;

	isSpawn_ = false;
	atkTimer_ = 0;

	isStunned_ = false;
	isInvincible_ = false;
	invincibleTimer_ = 0;

	pos_.x = 1600.0f;
	pos_.y = kGroundPos_ + 340.0f;
	screenPos_ = pos_;

	widthHalf_ = 30.0f;
	heightHalf_ = 88.0f;

	drawWidthHalf_ = 640.0f;
	drawHeightHalf_ = 360.0f;

	state_ = BOSS_IDLE;

	animePosX_ = 0;

	loop_ = 0;


	//graph_ = Novice::LoadTexture("./Resources/boss/sphinx.png");
	bodyGraph_ = Novice::LoadTexture("./Resources/boss/body.png");
	
	// left foot
	left.hp_ = footHP;
	left.startPos_ = { pos_.x - 322.0f, 180.0f };
	left.pos_.x = left.startPos_.x;
	left.pos_.y = left.startPos_.y;

	left.speed_ = { 0.0f, 0.0f };

	left.color_ = WHITE;

	left.widthHalf_ = kFootWidthHalf;
	left.heightHalf_ = kFootHeightHalf;

	left.drawWidthHalf_ = 112.0f;
	left.drawHeightHalf_ = 56.0f;

	isPartInvincible_ = false;
	partInvincibleTimer_ = 0;
	maxPartInvincibleTimer_ = 20;

	left.screenPos_ = left.pos_;
	leftFootGraph_ = Novice::LoadTexture("./Resources/boss/leftFoot.png");
	breakGraph_ = Novice::LoadTexture("./Resources/boss/footBreak.png");

	// right foot
	right.hp_ = footHP;
	right.startPos_ = { pos_.x + 322.0f, 180.0f };
	right.pos_.x = right.startPos_.x;
	right.pos_.y = right.startPos_.y;

	right.speed_ = { 0.0f, 0.0f };

	right.color_ = WHITE;

	right.widthHalf_ = kFootWidthHalf;
	right.heightHalf_ = kFootHeightHalf;

	right.drawWidthHalf_ = 112.0f;
	right.drawHeightHalf_ = 56.0f;

	right.screenPos_ = right.pos_;
	rightFootGraph_ = Novice::LoadTexture("./Resources/boss/rightFoot.png");

	// stomp
	isStomp_ = false;
	stompTimer_ = 0;
	stompCounter_ = 0;

	stomp.pos_.x = pos_.x - 322.0f;
	stomp.pos_.y = pos_.y - 312.0f;

	stomp.widthHalf_ = 200.0f;
	stomp.heightHalf_ = 36.0f;

	stomp.drawWidthHalf_ = 208.0f;
	stomp.drawHeightHalf_ = 56.0f;

	stomp.partsDirection_ = RIGHT;
	stomp.screenPos_ = stomp.pos_;
	stompGraph_ = Novice::LoadTexture("./Resources/boss/stomp.png");

	// gem
	gem.pos_ = { pos_.x, pos_.y + 281.0f };
	gem.screenPos_ = gem.pos_;

	gem.widthHalf_ = 38.0f;
	gem.heightHalf_ = 64.0f;

	gem.drawWidthHalf_ = 64.0f;
	gem.drawHeightHalf_ = 80.0f;
	color_ = WHITE;
	maxInvincibleTimer_ = 20;
	gemGraph_ = Novice::LoadTexture("./Resources/boss/gem.png");
	shineGraph_ = Novice::LoadTexture("./Resources/boss/shine.png");

	/// attacks
	rocks = new Rock;
	fireballs = new Fireball;
	fireballTimer_ = 0;

	// audio
	footSpinDamagedSFX_ = Novice::LoadAudio("./Resources/audio/sfx/spinDamaged.mp3");
	footSlashDamagedSFX_ = Novice::LoadAudio("./Resources/audio/sfx/enemyDamaged.mp3");
	footDestroyedSFX_ = Novice::LoadAudio("./Resources/audio/sfx/footDestroyed.mp3");

	stompSFX_ = Novice::LoadAudio("./Resources/audio/sfx/stomp.mp3");
}

Boss::~Boss() {
	delete rocks;
	delete fireballs;
}

void Boss::SpawnBoss() {
	hp_ = maxHP;
	right.hp_ = footHP;
	left.hp_ = footHP;

	isSpawn_ = false;
	atkTimer_ = 0;

	isStunned_ = false;
	isInvincible_ = false;
	invincibleTimer_ = 0;

	pos_.x = 1600.0f;
	pos_.y = kGroundPos_ + 340.0f;
	drawHeightHalf_ = 360.0f;

	state_ = BOSS_IDLE;

	loop_ = 0;
	fireballTimer_ = 0;
	fireballs->Reset();
	animePosX_ = 0;
}

void Boss::AtkManager() {
	if (state_ == BOSS_IDLE) {
		atkTimer_++;
	}
	// STOMP
	if (atkTimer_ == 120) {
		if (left.hp_ > 0 && right.hp_ > 0) {
			if (stompCounter_ % 2) {
				state_ = BOSS_LEFT_ATK;
			} else {
				state_ = BOSS_RIGHT_ATK;
			}
		} else if (left.hp_ <= 0) {
			state_ = BOSS_RIGHT_ATK;
		} else if (right.hp_ <= 0) {
			state_ = BOSS_LEFT_ATK;
		}
	}

	// foot / stomp attack
	if (left.hp_ > 0) {
		left.pos_.y += left.speed_.y;
		LeftAtk();
	} else {
		left.pos_.y = 180.0f;
	}

	if (right.hp_ > 0) {
		right.pos_.y += right.speed_.y;
		RightAtk();
	} else {
		right.pos_.y = 180.0f;
	}

	if (left.hp_ <= 0 && right.hp_ <= 0) {
		isStunned_ = true;
		animePosX_ = 1280;
	} else {
		animePosX_ = 0;
	}

	// stomp Reset
	if (isStomp_ == true) {
		if (atkTimer_ >= 20) {
			isStomp_ = false;
		}
	}



	if (hp_ <= 0) {
		pos_.y -= 2;
	} else {

		if (hp_ <= 80) {
			if (loop_ == 0) {
				rocks->isLaunched_ = true;
				isInvincible_ = true;
				isStunned_ = false;
				left.hp_ = footHP;
				right.hp_ = footHP;
				maxInvincibleTimer_ = 240;
				loop_ += 1;
			}
		}

		if (hp_ <= 60) {
			if (loop_ == 1) {
				rocks->isLaunched_ = true;
				isInvincible_ = true;
				isStunned_ = false;
				left.hp_ = footHP;
				right.hp_ = footHP;
				maxInvincibleTimer_ = 240;
				loop_ += 1;

			}

			if (loop_ == 2) {
				if (fireballs->isLaunched_ == false) {
					fireballTimer_++;
				}
				if (fireballTimer_ == 60) {
					fireballs->isLaunched_ = true;

					loop_++;
				}
			}


			if (loop_ >= 3) {
				fireballs->cooldown_--;
				if (fireballs->cooldown_ == 0) {
					fireballs->cooldown_ = 300;
					fireballTimer_ = 0;
				}
				if (fireballs->isLaunched_ == false) {
					fireballTimer_++;
				}
				if (fireballTimer_ == 60) {
					fireballs->isLaunched_ = true;
				}
			}
		}

		if (hp_ <= 30) {
			if (loop_ == 3) {
				rocks->isLaunched_ = true;
				isInvincible_ = true;
				isStunned_ = false;
				left.hp_ = footHP;
				right.hp_ = footHP;
				maxInvincibleTimer_ = 240;
				loop_ += 1;
			}
		}
	}

	
}

void Boss::LeftAtk() {
	if (state_ == BOSS_LEFT_ATK) {
		left.speed_.y = kStompSpeedUp;
		if (left.pos_.y >= 400.0f) {
			left.speed_.y = 0;
			stompTimer_++;
		}

		if (stompTimer_ >= 60) {
			left.speed_.y = kStompSpeedDown;
			if (left.pos_.y <= 196.0f) {
				Novice::PlayAudio(stompSFX_, 0, 1.0f);
				left.pos_.y = 196.0f;
				state_ = BOSS_IDLE;
				atkTimer_ = 0;
				stompTimer_ = 0;
				stompCounter_++;

				isStomp_ = true;
				stomp.pos_.x = left.pos_.x;
				stomp.pos_.y = left.pos_.y - 12.0f;
				stomp.partsDirection_ = RIGHT;
			}
		}
		
	} else {
		left.speed_.y = 0.0f;
	}
}

void Boss::RightAtk() {
	if (state_ == BOSS_RIGHT_ATK) {
		right.speed_.y = kStompSpeedUp;
		if (right.pos_.y >= 400.0f) {
			right.speed_.y = 0;
			stompTimer_++;
		}

		if (stompTimer_ >= 60) {
			right.speed_.y = kStompSpeedDown;
			if (right.pos_.y <= 196.0f) {
				Novice::PlayAudio(stompSFX_, 0, 1.0f);
				right.pos_.y = 196.0f;
				state_ = BOSS_IDLE;
				atkTimer_ = 0;
				stompTimer_ = 0;
				stompCounter_++;

				isStomp_ = true;
				stomp.pos_.x = right.pos_.x;
				stomp.pos_.y = right.pos_.y - 12.0f;
				stomp.partsDirection_ = LEFT;
			}
		}

	} else {
		right.speed_.y = 0.0f;
	}
}

bool Boss::ChakramCollision(Chakram* chakram) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();

	if (isInvincible_ == false && state_ != BOSS_DEAD && chakram->GetState() >= LAUNCH && isStunned_ == true) {
		if (hp_ > 0) {
			if (gem.pos_.x - gem.widthHalf_ < chakramPosX + chakramWidthHalf && gem.pos_.x + gem.widthHalf_ > chakramPosX - chakramWidthHalf) {
				if (gem.pos_.y - gem.heightHalf_ < chakramPosY + chakramHeightHalf && gem.pos_.y + gem.heightHalf_ > chakramPosY - chakramHeightHalf) {
					Novice::PlayAudio(footSpinDamagedSFX_, 0, 1.0f);
					isInvincible_ = true;
					color_ = BLACK;
					hp_--;
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

bool Boss::SlashCollision(PlayerAttack* slash) {
	float slashPosX = slash->GetPosX();
	float slashPosY = slash->GetPosY();

	float slashWidthHalf = slash->GetWidthHalf();
	float slashHeightHalf = slash->GetHeightHalf();

	if (isInvincible_ == false && state_ != BOSS_DEAD && isStunned_ == true) {
		if (hp_ > 0) {
			if (gem.pos_.x - gem.widthHalf_ < slashPosX + slashWidthHalf && gem.pos_.x + gem.widthHalf_ > slashPosX - slashWidthHalf) {
				if (gem.pos_.y - gem.heightHalf_ < slashPosY + slashHeightHalf && gem.pos_.y + gem.heightHalf_ > slashPosY - slashHeightHalf) {
					Novice::PlayAudio(footSlashDamagedSFX_, 0, 1.0f);
					isInvincible_ = true;
					color_ = BLACK;
					hp_ -= 3;
					maxInvincibleTimer_ = 40;
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

bool Boss::FootChakramCollision(Chakram* chakram) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();

	if (isPartInvincible_ == false && state_ != BOSS_DEAD && chakram->GetState() >= LAUNCH) {
		if (left.hp_ > 0) {
			if (left.pos_.x - left.widthHalf_ < chakramPosX + chakramWidthHalf && left.pos_.x + left.widthHalf_ > chakramPosX - chakramWidthHalf) {
				if (left.pos_.y - left.heightHalf_ < chakramPosY + chakramHeightHalf && left.pos_.y + left.heightHalf_ > chakramPosY - chakramHeightHalf) {
					Novice::PlayAudio(footSpinDamagedSFX_, 0, 1.0f);
					isPartInvincible_ = true;
					left.hp_--;
					left.color_ = RED;
					maxPartInvincibleTimer_ = 20;
					
					return true;
				}
			}
		}

		if (right.hp_ > 0) {
			if (right.pos_.x - right.widthHalf_ < chakramPosX + chakramWidthHalf && right.pos_.x + right.widthHalf_ > chakramPosX - chakramWidthHalf) {
				if (right.pos_.y - right.heightHalf_ < chakramPosY + chakramHeightHalf && right.pos_.y + right.heightHalf_ > chakramPosY - chakramHeightHalf) {
					Novice::PlayAudio(footSpinDamagedSFX_, 0, 1.0f);
					isPartInvincible_ = true;
					right.hp_--;
					right.color_ = RED;
					maxPartInvincibleTimer_ = 20;
					
					return true;
				}
			}
		}
	} else {
		partInvincibleTimer_++;
		if (partInvincibleTimer_ > 10) {
			right.color_ = WHITE;
			left.color_ = WHITE;
		}
		if (partInvincibleTimer_ > maxPartInvincibleTimer_) {
			partInvincibleTimer_ = 0;
			isPartInvincible_ = false;
		}
	}

	return false;
}

bool Boss::FootSlashCollision(PlayerAttack* slash) {
	float slashPosX = slash->GetPosX();
	float slashPosY = slash->GetPosY();

	float slashWidthHalf = slash->GetWidthHalf();
	float slashHeightHalf = slash->GetHeightHalf();

	if (isPartInvincible_ == false && state_ != BOSS_DEAD) {
		if (left.hp_ > 0) {
			if (left.pos_.x - left.widthHalf_ < slashPosX + slashWidthHalf && left.pos_.x + left.widthHalf_ > slashPosX - slashWidthHalf) {
				if (left.pos_.y - left.heightHalf_ < slashPosY + slashHeightHalf && left.pos_.y + left.heightHalf_ > slashPosY - slashHeightHalf) {
					Novice::PlayAudio(footSlashDamagedSFX_, 0, 1.0f);
					isPartInvincible_ = true;
					left.hp_-= 3;
					left.color_ = RED;
					maxPartInvincibleTimer_ = 40;
					return true;
				}
			}
		}

		if (right.hp_ > 0) {
			if (right.pos_.x - right.widthHalf_ < slashPosX + slashWidthHalf && right.pos_.x + right.widthHalf_ > slashPosX - slashWidthHalf) {
				if (right.pos_.y - right.heightHalf_ < slashPosY + slashHeightHalf && right.pos_.y + right.heightHalf_ > slashPosY - slashHeightHalf) {
					Novice::PlayAudio(footSlashDamagedSFX_, 0, 1.0f);
					isPartInvincible_ = true;
					right.hp_-= 3;
					right.color_ = RED;
					maxPartInvincibleTimer_ = 40;
					return true;
				}
			}
		}
	} else {
		partInvincibleTimer_++;
		if (partInvincibleTimer_ > 10) {
			right.color_ = WHITE;
			left.color_ = WHITE;
		}
		if (partInvincibleTimer_ > maxPartInvincibleTimer_) {
			partInvincibleTimer_ = 0;
			isPartInvincible_ = false;
		}
	}

	return false;
}

void Boss::Update(const Player * player) {
	pos_.x = 1600.0f;
	

	AtkManager();

	rocks->Update(player->GetScrollPos());
	if (hp_ > 0) {
		fireballs->Update(player->GetScrollPos());
	} else {
		fireballs->Reset();
	}


	screenPos_.x = pos_.x - player->GetScrollPosX();
	screenPos_.y = pos_.y - player->GetScrollPosY();

	left.screenPos_.x = left.pos_.x - player->GetScrollPosX();
	left.screenPos_.y = left.pos_.y - player->GetScrollPosY();

	right.screenPos_.x = right.pos_.x - player->GetScrollPosX();
	right.screenPos_.y = right.pos_.y - player->GetScrollPosY();

	/// stomp
	stomp.screenPos_.x = stomp.pos_.x - player->GetScrollPosX();
	stomp.screenPos_.y = stomp.pos_.y - player->GetScrollPosY();

	// gem
	gem.screenPos_.x = gem.pos_.x - player->GetScrollPosX();
	gem.screenPos_.y = gem.pos_.y - player->GetScrollPosY();
}

void Boss::Draw(int shakeX, int shakeY) const {
	// body
	DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, 0, bodyGraph_, direction_, shakeX, shakeY);

	// stomp
	if (isStomp_ == true) {
		DrawObject(stomp.screenPos_, stomp.drawWidthHalf_, stomp.drawHeightHalf_, 0, 0, stompGraph_, direction_, 0, 0);

	/*	Novice::DrawBox(static_cast<int>(stomp.screenPos_.x - stomp.widthHalf_), static_cast<int>(ConvertToScreen(stomp.screenPos_.y) + stomp.heightHalf_),
			static_cast<int>(stomp.widthHalf_ * 2.f), static_cast<int>(-(stomp.heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/
	}


	// left Foot
	if (left.hp_ > 0) {
		DrawObject(left.screenPos_, left.drawWidthHalf_, left.drawHeightHalf_, 0, 0, leftFootGraph_, RIGHT, 0, 0,left.color_);
		/*Novice::DrawBox(static_cast<int>(left.screenPos_.x - left.widthHalf_), static_cast<int>(ConvertToScreen(left.screenPos_.y) + left.heightHalf_),
			static_cast<int>(left.widthHalf_ * 2.f), static_cast<int>(-(left.heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

	} else {
		DrawObject({ left.screenPos_.x,left.screenPos_.y + 4.0f }, left.drawWidthHalf_, left.drawHeightHalf_, 0, 0, breakGraph_, RIGHT, 0, 0);
	}

	// right Foot
	if (right.hp_ > 0) {
		DrawObject(right.screenPos_, right.drawWidthHalf_, right.drawHeightHalf_, 0, 0, rightFootGraph_, RIGHT, 0, 0, right.color_);
	} else {
		DrawObject({ right.screenPos_.x,right.screenPos_.y  + 4.0f}, right.drawWidthHalf_, right.drawHeightHalf_, 0, 0, breakGraph_, LEFT, 0, 0);
	}

	// gem
	if (hp_ > 0) {
		if (isStunned_ == true) {
			// shine
			DrawObject(gem.screenPos_, gem.drawWidthHalf_, gem.drawHeightHalf_, 0, 0, shineGraph_, RIGHT, 0, 0, WHITE);
		}
		DrawObject(gem.screenPos_, gem.drawWidthHalf_, gem.drawHeightHalf_, 0, 0, gemGraph_, RIGHT, shakeX, shakeY, color_);
	}

	/*Novice::DrawBox(static_cast<int>(gem.screenPos_.x - gem.widthHalf_), static_cast<int>(ConvertToScreen(gem.screenPos_.y) + gem.heightHalf_),
		static_cast<int>(gem.widthHalf_ * 2.f), static_cast<int>(-(gem.heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

	// Rocks
	rocks->Draw();
	fireballs->Draw();

	

	//Novice::ScreenPrintf(0, 200, "pos X %0.2f Y %0.2f", pos_.x, pos_.y);
	/*Novice::ScreenPrintf(180, 0, "atkTimer %d stomp %d count %d", atkTimer_, stompTimer_, stompCounter_);
	Novice::ScreenPrintf(180, 20, "isStunned_ %d", isStunned_);
	Novice::ScreenPrintf(520, 0, "BOSS HP %d", hp_);
	Novice::ScreenPrintf(520, 20, "LEFT %d RIGHT %d", left.hp_, right.hp_);
	Novice::ScreenPrintf(640, 0, "invin %d timer %d loop %d", isInvincible_, invincibleTimer_, loop_);
	Novice::ScreenPrintf(640, 20, "fireballTimer_ %d", fireballTimer_);*/


	/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
		static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/
}
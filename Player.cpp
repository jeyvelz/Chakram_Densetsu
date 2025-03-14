#include "Player.h"
#include "Novice.h"
#include "ToScreen.h"
#include "ToTile.h"

#include "ImGuiManager.h"

const int kImageWidth = 144;
const int kImageHeight = 208;
const int kMaxHP = 5;

const int kMaxAtkFrame = 12;

Player::Player() {
	isSpawn_ = false;
	hp_ = kMaxHP;
	state_ = IDLE;
	pos_.x = 120.f; //120
	pos_.y = 240.f; // 240
	prePos_ = pos_;
	
	isPressLeft_ = false;
	isPressRight_ = false;

	canMoveLeft_ = false;
	canMoveRight_ = false;

	isOnPlatform_ = false;
	isGround_ = false;
	isAtk_ = false;

	screenPos_.x = pos_.x;
	screenPos_.y = ConvertToScreen(pos_.y);

	jumpHeight_ = 16.0f;
	direction_ = RIGHT;

	scrollPos_.x = 0.f;
	scrollPos_.y = 0.f;
	speed_.x = 6.4f;
	speed_.y = 0.f;

	knockback_ = { 8.2f, 0.0f };
	atkMovement_ = { 6.4f, 0.0f };
	invincibleFrame_ = 0;
	kInvincibleFrame_ = 90;

	maxAtkFrame_ = 2;
	maxAtkTimer_ = kMaxAtkFrame;
	atkTimer_ = 0;
	atkHit_ = 0;
	atkFrame_ = 0;

	chargeTimer_ = 0;
	throwCD_ = 0;

	widthHalf_ = 16.f; // HitBox Size
	heightHalf_ = 62.f;
	drawWidthHalf_ = 72.f; // Image Size
	drawHeightHalf_ = 104.f;

	animePosX_ = 0;
	animePosY_ = 0;
	animeFrame_ = 0;
	
	isTopCollision_ = false;

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

	noChakramGraph_ = Novice::LoadTexture("./Resources/player_nochakram.png");
	chakramGraph_ = Novice::LoadTexture("./Resources/player.png");

	// audio
	damagedSFX = Novice::LoadAudio("./Resources/audio/sfx/damaged.mp3");
	swing1SFX = Novice::LoadAudio("./Resources/audio/sfx/swing1.mp3");
	swing2SFX = Novice::LoadAudio("./Resources/audio/sfx/swing2.mp3");
	jumpSFX = Novice::LoadAudio("./Resources/audio/sfx/jump.mp3");

	graph_ = chakramGraph_;
	chakram_ = new Chakram();
	slash_ = new PlayerAttack();
}

Player::~Player() {
	delete chakram_;
	delete slash_;
}

void Player::MapCollisionDetection(int(*map)[kMapWidth]) {
	canMoveLeft_ = false;
	canMoveRight_ = false;


	// LeftCollision
	if(map[BlockY(leftTopOld_.y)][BlockX(leftTop_.x)] == 0 &&
		map[BlockY(leftBottomOld_.y)][BlockX(leftBottom_.x)] == 0 &&
		map[BlockY(leftBottomOld_.y) - 1][BlockX(leftBottom_.x)] == 0){
		canMoveLeft_ = true;
	}


	// RightCollision
	if (map[BlockY(rightTopOld_.y)][BlockX(rightTop_.x)] == 0 &&
		map[BlockY(rightBottomOld_.y)][BlockX(rightBottom_.x)] == 0 &&
		map[BlockY(rightBottomOld_.y) - 1][BlockX(rightBottom_.x)] == 0) {
		canMoveRight_ = true;
	}



	
	if (canMoveLeft_ == true) {
		
		if (isPressLeft_) {
			pos_.x = prePos_.x - speed_.x;
		}
		
	} else {
		pos_.x = static_cast<float>((BlockX(leftBottom_.x) + 1) * kBlockSize + widthHalf_);
		
	}


	if (canMoveRight_ == true) {

		if (isPressRight_) {
			pos_.x = prePos_.x + speed_.x;
		}

	} else {
		pos_.x = static_cast<float>(BlockX(rightBottom_.x) * kBlockSize - widthHalf_);

	}
}

void Player::HorizontalMove(char* keys) {
	isPressLeft_ = false;
	isPressRight_ = false;

	if (keys[DIK_LEFT]) {
		if (state_ == IDLE) {
			if (state_ != DAMAGED) {
				pos_.x -= speed_.x;
				isPressLeft_ = true;
				direction_ = LEFT;
				state_ = WALK;
			}
		}
	} else if (keys[DIK_RIGHT]){
		if (state_ == IDLE) {
			if (state_ != DAMAGED) {
				pos_.x += speed_.x;
				isPressRight_ = true;
				direction_ = RIGHT;
				state_ = WALK;
			}
		}
	}

}

bool Player::BoxCollision(Vector2 const& pos, float const& widthHalf, float const& heightHalf) {
	if (pos_.x - widthHalf_ < pos.x + widthHalf && pos_.x + widthHalf_ > pos.x - widthHalf) {
		if (pos_.y - heightHalf_ < pos.y + heightHalf && pos_.y + heightHalf_ > pos.y - heightHalf) {
			
			return true;
		}
	}
	
	return false;
}



void Player::Move(char* keys, char* preKeys, int(*map)[kMapWidth]) {

	prePos_.x = pos_.x;
	prePos_.y = pos_.y;

	// Melee
	/*if (keys[DIK_X] && !preKeys[DIK_X]) {
		if (chakram_->GetState() == NONE) {
			if (state_ != ATTACK && state_ != CHARGE && state_ != THROW_ATTACK && isGround_) {
				state_ = ATTACK;
				animeFrame_ = 0;
				animePosX_ = 0;
			}
		}
	}*/

	// Melee 3 combo
	if (keys[DIK_X] && !preKeys[DIK_X]) {
		if (chakram_->GetState() == NONE) {
			if (state_ != CHARGE && state_ != THROW_ATTACK && isGround_) {
				if (atkHit_ != 2) {
					state_ = ATTACK;
					animePosY_ = kImageHeight * 2;
					if (atkHit_ > 0) {
						Novice::PlayAudio(swing1SFX, 0, 0.7f);
						maxAtkFrame_ = 4;
						maxAtkTimer_ = kMaxAtkFrame * 2;
						animePosX_ = kImageWidth;
						
					} else {
						Novice::PlayAudio(swing2SFX, 0, 0.7f);
						maxAtkFrame_ = 2;
						animePosX_ = 0;
						maxAtkTimer_ = kMaxAtkFrame;
						//pos_.x += atkMovement_.x;
					}

				}
				atkHit_++;
			}
		}
	}

	if (state_ == ATTACK) {
		slash_->Attack(pos_, direction_, scrollPos_);
		slash_->isAtk_ = true;
		atkTimer_++;
		if (atkTimer_ % 6 == 0) {
			animePosX_ += kImageWidth;
		}

		if (animePosX_ >= kImageWidth * maxAtkFrame_) {
			animePosX_ = 0;
		}

		if (atkTimer_ >= maxAtkTimer_) {
			atkHit_ = 0;
			state_ = IDLE;
			atkTimer_ = 0;
			//pos_.x -= atkMovement_.x;
		}
	} else {
		slash_->isAtk_ = false;
		slash_->Reset();
	}


	// Charge Atk
	if (keys[DIK_C]) {
		if (chakram_->GetState() == NONE) {
			if (state_ != CHARGE && isGround_ && state_ != DAMAGED) {
				state_ = CHARGE;
			}
		}

		if (state_ == CHARGE) {
			chargeTimer_++;
		}

		if (state_ == DAMAGED) {
			chargeTimer_ = 0;
			chakram_->SetState(NONE);
		}

		if (chargeTimer_ == 20) {
			chakram_->SetState(PREAIM);

		} else if (chargeTimer_ > 20) {
			chakram_->SetState(AIM);

			if (state_ == DAMAGED) {
				chargeTimer_ = 0;
				chakram_->SetState(NONE);
			}
		}

		if (chakram_->GetTargetPosX() >= pos_.x) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}
	}

	if (!keys[DIK_C] && preKeys[DIK_C]) {
		if (chargeTimer_ >= 20) {
			state_ = THROW_ATTACK;
			chakram_->SetState(LAUNCH);
			chargeTimer_ = 0;
		} else {
			state_ = IDLE;
			chargeTimer_ = 0;
		}
	}


	if (throwCD_ >= 30) {
		throwCD_ = 0;
		state_ = IDLE;
	}

	if (invincibleFrame_ > 0) {
		invincibleFrame_--;
	}

	if (state_ == DAMAGED) {
		if (direction_ == RIGHT) {
			pos_.x -= knockback_.x;
		} else {
			pos_.x += knockback_.x;
		}
		if (animeFrame_ >= 16) {
			state_ = IDLE;
		}
	}

	if (state_ != DAMAGED) {
		if (state_ != ATTACK && state_ != CHARGE && state_ != THROW_ATTACK) {
			state_ = IDLE;
		}
	}

	// Jump
	if (keys[DIK_Z] && !preKeys[DIK_Z] && state_ != CHARGE && state_ != THROW_ATTACK && state_ != DAMAGED
		&& isGround_ == true) {
		Novice::PlayAudio(jumpSFX, 0, 1.0f);
		speed_.y = jumpHeight_;
	}

	if (preKeys[DIK_Z] && !keys[DIK_Z] && speed_.y > 0.f) {
		speed_.y *= 0.45f;
	}

	
	/*if (keys[DIK_A] && !preKeys[DIK_A]) {
		isOpen = true;
	}

	if (isOpen == true) {
		for (int i = 0; i < kMapHeight; i++) {
			for (int j = 0; j < kMapWidth; j++) {
				if (map[i][j] >= 30) {
					map[i][j] = 0;
				}
			}
		}
	}*/


	isOnPlatform_ = false;
	isGround_ = false;
	speed_.y += kGravity;

	// BottomCollision
	if (map[BlockY(leftBottom_.y + speed_.y - 1.0f)][BlockX(leftBottom_.x)] >= 1 ||
		map[BlockY(rightBottom_.y + speed_.y - 1.0f)][BlockX(rightBottom_.x)] >= 1) {
		isGround_ = true;
		speed_.y = 0.0f;
		pos_.y = ConvertToScreen(static_cast<float>((BlockY(leftBottom_.y) + 1) * kBlockSize - heightHalf_));
	} else {
		pos_.y += speed_.y;
	}

	// TopCollision
	if (map[BlockY(leftTop_.y + speed_.y - 1.0f)][BlockX(leftTop_.x)] >= 1 ||
		map[BlockY(rightTop_.y + speed_.y - 1.0f)][BlockX(rightTop_.x)] >= 1) {

		if (speed_.y > 0.0f) {
			speed_.y *= -0.15f;
		}

		pos_.y = prePos_.y;
		isTopCollision_ = true;
	} else {
		isTopCollision_ = false;
	}

	HorizontalMove(keys);

	VertexUpdate();
	MapCollisionDetection(map);

	VertexUpdate();

	// Set Image with chakram or not
	if (chakram_->GetState() <= AIM) {
		graph_ = chakramGraph_;
	} else {
		graph_ = noChakramGraph_;
	}

	/// Stand on chakram collision
	if (chakram_->GetState() == SPINNING || chakram_->GetState() == STUCK) {
		if (isTopCollision_ == false) {
			if (BoxCollision({ chakram_->GetPosX(),chakram_->GetPosY() },
				chakram_->GetWidthHalf(), chakram_->GetHeightHalf()) == true) {
				if (speed_.y < 0.0f && pos_.y > chakram_->GetPosY()) {
					speed_.y = 0.0f;
					isGround_ = true;
					pos_.y = chakram_->GetPosY() + heightHalf_;
				}
			}
		}
	}

	if (pos_.x <= 0.0f) {
		pos_.x = 200.0f;
	}

	if (pos_.y >= 1300.0f) {
		pos_.y = 1250.0f;
	}

	Scroll();

	screenPos_.x = pos_.x - scrollPos_.x;
	screenPos_.y = pos_.y - scrollPos_.y;

	chakram_->Update(keys, preKeys, pos_, scrollPos_, direction_, map);

	AnimeUpdate();
	
}

void Player::Scroll(){
	// Horizontal Scroll
	if (screenPos_.x >= kScrollStartX) {
		scrollPos_.x = pos_.x - kScrollStartX;

		if (scrollPos_.x >= kScrollEndX) {
			scrollPos_.x = kScrollEndX;
		}
		if (scrollPos_.x < 0.0f) {
			scrollPos_.x = 0.0f;
		}
	} else {
		if (speed_.x < 0.0f) {
			screenPos_.x += speed_.x;
			if (scrollPos_.x < 0.0f) {
				scrollPos_.x = 0.0f;
			}
		}
	}

	// Vertical Scroll
	if (screenPos_.y >= kScrollTopStartY) {
		scrollPos_.y = pos_.y - kScrollTopStartY;

		if (scrollPos_.y >= kScrollEndY) {
			scrollPos_.y = kScrollEndY;
		}
		if (scrollPos_.y <= 0.0f) {
			scrollPos_.y = 0.0f;
		}
	} else {
		if (speed_.y < 0.0f) {
			scrollPos_.y += speed_.y;
			if (scrollPos_.y < 0.0f) {
				scrollPos_.y = 0.0f;
			}
		}
	}
	scrollPos_.y -= kScrollEndY;
}

void Player::VertexUpdate() {
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

void Player::Damaged(int damage) {
	if (invincibleFrame_ <= 0) {
		state_ = DAMAGED;
		hp_ -= damage;
		invincibleFrame_ = kInvincibleFrame_;
		animeFrame_ = 0;
		animePosX_ = 0;
		speed_.y = 0.f;
		Novice::PlayAudio(damagedSFX, 0, 1.0f);
	}
}


void Player::AnimeUpdate() {
	animeFrame_++;
	if (speed_.y) {
		if (state_ == DAMAGED) {
			animePosY_ = kImageHeight * 6;
		} else {
			/// Jump Fall Animation
			if (speed_.y > 0.0f) {
				state_ = JUMP;
				animePosY_ = kImageHeight * 4;

			} else if (speed_.y < 0.0f) {
				state_ = FALL;
				animePosY_ = kImageHeight * 5;
			}
				

			if (animeFrame_ % 8 == 0) {
				animePosX_ += kImageWidth;

			}
			if (animePosX_ >= kImageWidth * 2) {
				animePosX_ = 0;
			}
		}
	} else {
		
		switch (state_) {
		case(IDLE):
			animePosY_ = 0;
			if (animeFrame_ % 6 == 0) {
				animePosX_ += kImageWidth;

			}
			if (animePosX_ >= kImageWidth * 4) {
				animePosX_ = 0;
			}
			break;
		case(WALK):
			animePosY_ = kImageHeight;
			if (animeFrame_ % 6 == 0) {
				animePosX_ += kImageWidth;

			}
			if (animePosX_ >= kImageWidth * 5) {
				animePosX_ = 0;
			}
			break;
		case(JUMP):

			break;
		case(FALL):

			break;
		case(ATTACK):
			/*animePosY_ = kImageHeight * 2;
			if (animeFrame_ % 6 == 0) {
				animePosX_ += kImageWidth;
			}

			if (animePosX_ >= kImageWidth * 2) {
				animePosX_ = 0;
				state_ = IDLE;
			}*/
			animePosY_ = kImageHeight * 2;
			

			break;
		case(CHARGE):
			animePosY_ = kImageHeight * 3;
			if (animeFrame_ % 4 == 0) {
				animePosX_ += kImageWidth;
			}

			if (animePosX_ >= kImageWidth * 4) {
				animePosX_ = 0;
			}

			break;
		case(THROW_ATTACK):
			throwCD_++;
			animePosY_ = kImageHeight * 3;
			animePosX_ = kImageWidth * 4;

			break;
		case(DAMAGED):
			animePosY_ = kImageHeight * 6;
			break;
		}
	}
}

void Player::Draw() {
	if (isSpawn_ == true) {
		if (invincibleFrame_ % 4 == 0) {
			DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, direction_, 0, 0);
		}
		// box collision
		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/

		//slash_->DrawBoxCollision();
		chakram_->DrawChakram();
	}

	/*ImGui::Begin("Player Pos");
	ImGui::Text("Screen x : %.2f, y : %.2f", screenPos_.x, ConvertToScreen(screenPos_.y));
	ImGui::Text("Pos x : %.2f, y : %.2f", pos_.x, pos_.y);
	ImGui::End();*/

	//Novice::ScreenPrintf(0, 200, "pos X %0.2f Y %0.2f", pos_.x, pos_.y);
	
	/*Novice::ScreenPrintf(1000, 60, "LB x %d LB y %d ", BlockX(vertex_.leftBottom.x), BlockY(vertex_.leftBottom.y));
	Novice::ScreenPrintf(1000, 200, "scroll X %0.2f Y %0.2f", scrollPos_.x, scrollPos_.y);*/
	//Novice::ScreenPrintf(200, 0, "topCol %d", isTopCollision_);
	
	/*Novice::ScreenPrintf(570, 40, "state %d", state_);
	Novice::ScreenPrintf(700, 40, "iFrame %d", invincibleFrame_);

	Novice::ScreenPrintf(600, 60, "isGround_ %d speed_.y %0.2f", isGround_, speed_.y);
	Novice::ScreenPrintf(600, 80, "animeposY %d isOnPlatform_ %d", animePosY_, isOnPlatform_);
	Novice::ScreenPrintf(600, 100, "ATK Timer %d anime %d", atkTimer_, atkHit_);

	Novice::ScreenPrintf(900, 60, "LTx %0.2f LTy %0.2f ", leftTop_.x, leftTop_.y);
	Novice::ScreenPrintf(1130, 60, "RTx %0.2f RTy %0.2f ", rightTop_.x, rightTop_.y);
	Novice::ScreenPrintf(900, 90, "LBx %d LBy %d ", BlockX(leftBottom_.x), int(leftBottom_.y / kBlockSize));
	Novice::ScreenPrintf(1130, 90, "RBx %d RBy %d ", BlockX(rightBottom_.x), int(rightBottom_.y / kBlockSize));*/
}

void Player::SpawnPlayer(Vector2 pos) {
	if (isSpawn_ == false) {
		isSpawn_ = true;
		pos_ = pos;
		state_ = IDLE;
		direction_ = RIGHT;
		hp_ = kMaxHP;
		speed_.x = 6.4f;
		speed_.y = 0.f;

		atkTimer_ = 0;
		atkHit_ = 0;
		chargeTimer_ = 0;
		throwCD_ = 0;

		animePosX_ = 0;
		animePosY_ = 0;
		animeFrame_ = 0;

		invincibleFrame_ = 0;

		isPressLeft_ = false;
		isPressRight_ = false;

		canMoveLeft_ = false;
		canMoveRight_ = false;

		isGround_ = false;
		isAtk_ = false; 
	}
}

void Player::DespawnPlayer() {
	isSpawn_ = false;
	isGround_ = false;
	speed_.y = 0.0f;

	chakram_->SetState(NONE);
	pos_ = { -999.0f, -299.0f };
}

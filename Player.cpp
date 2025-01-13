#include "Player.h"
#include "Novice.h"
#include "ToScreen.h"
#include "ToTile.h"

const int kImageWidth = 144;
const int kImageHeight = 208;

Player::Player() {
	state_ = IDLE;
	pos_.x = 120.f; //120
	pos_.y = 240.f; // 150
	prePos_ = pos_;
	
	isPressLeft_ = false;
	isPressRight_ = false;

	canMoveLeft_ = false;
	canMoveRight_ = false;

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

	chargeTimer_ = 0;
	throwCD_ = 0;

	widthHalf_ = 16.f; // HitBox Size
	heightHalf_ = 62.f;
	drawWidthHalf_ = 72.f; // Image Size
	drawHeightHalf_ = 104.f;

	animePosX_ = 0;
	animePosY_ = 0;
	animeFrame_ = 0;

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


	graph_ = Novice::LoadTexture("./Resources/player.png");
	chakram_ = new Chakram();
	isChakramHit = false;
}

Player::~Player() {
	delete chakram_;
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
			pos_.x -= speed_.x;
			isPressLeft_ = true;
			direction_ = LEFT;
			state_ = WALK;
		}
	} else if (keys[DIK_RIGHT]){
		if (state_ == IDLE) {
			pos_.x += speed_.x;
			isPressRight_ = true;
			direction_ = RIGHT;
			state_ = WALK;
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
	if (keys[DIK_X] && !preKeys[DIK_X]) {
		if (state_ != ATTACK && state_ != CHARGE && state_ != THROW_ATTACK && isGround_) {
			state_ = ATTACK;
			animeFrame_ = 0;
			animePosX_ = 0;
		}
	}

	// Charge Atk
	
		if (keys[DIK_C]) {
			if (state_ != CHARGE && isGround_) {
				state_ = CHARGE;
			}

			if (state_ == CHARGE) {
				chargeTimer_++;
			}
			if (chargeTimer_ == 20) {
				chakram_->SetState(PREAIM);

			} else if (chargeTimer_ > 20) {
				chakram_->SetState(AIM);
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

	if (state_ != ATTACK && state_ != CHARGE && state_ != THROW_ATTACK) {
		state_ = IDLE;
	}

	// Jump
	if (keys[DIK_Z] && !preKeys[DIK_Z] && state_ != CHARGE && state_ != THROW_ATTACK /* && isGround_ == true*/) {
		speed_.y = jumpHeight_;
	}

	if (preKeys[DIK_Z] && !keys[DIK_Z] && speed_.y > 0.f) {
		speed_.y *= 0.45f;
	}
	
	isGround_ = false;
	speed_.y += kGravity;

	// BottomCollision
	if (map[BlockY(leftBottom_.y + speed_.y - 1.0f)][BlockX(leftBottom_.x)] == 1 ||
		map[BlockY(rightBottom_.y + speed_.y - 1.0f)][BlockX(rightBottom_.x)] == 1) {
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
			speed_.y *= -0.45f;
		}

		pos_.y = prePos_.y;
	}

	HorizontalMove(keys);

	VertexUpdate();
	MapCollisionDetection(map);

	VertexUpdate();

	if (chakram_->GetState() == SPINNING || chakram_->GetState() == STUCK ) {
		if (BoxCollision({ chakram_->GetPosX(),chakram_->GetPosY() },
			chakram_->GetWidthHalf(), chakram_->GetHeightHalf()) == true) {
			isChakramHit = true;
			if (speed_.y < 0.0f && pos_.y > chakram_->GetPosY()) {
				speed_.y = 0.0f;
				isGround_ = true;
				pos_.y = chakram_->GetPosY() + heightHalf_;
			}
		}
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

void Player::AnimeUpdate() {
	animeFrame_++;
	if (speed_.y) {
		if(state_ == ATTACK){
			animePosY_ = kImageHeight * 2;
			if (animeFrame_ % 4 == 0) {
				animePosX_ += kImageWidth;
			}
			
			if (animePosX_ >= kImageWidth * 3) {
				animePosX_ = 0;
				state_ = IDLE;
			}
		} else {
			/// Jump Fall Animation
			if (speed_.y > 0.0f) {
				animePosY_ = kImageHeight * 4;

			} else if (speed_.y < 0.0f) {
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
			animePosY_ = kImageHeight * 2;
			if (animeFrame_ % 4 == 0) {
				animePosX_ += kImageWidth;
			}

			if (animePosX_ >= kImageWidth * 3) {
				animePosX_ = 0;
				state_ = IDLE;
			}

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
		}
	}
}

void Player::Draw() {
	if (direction_ == RIGHT) {
		Novice::DrawQuad(static_cast<int>(screenPos_.x - drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x - drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			animePosX_, animePosY_, static_cast<int>(drawWidthHalf_ * 2.f), static_cast<int>(drawHeightHalf_ * 2.f), graph_, WHITE);
	} else {
		Novice::DrawQuad(static_cast<int>(screenPos_.x - drawWidthHalf_ * -1.0f) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_ * -1.0f) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x - drawWidthHalf_ * -1.0f) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_ * -1.0f) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			animePosX_, animePosY_, static_cast<int>(drawWidthHalf_ * 2.f), static_cast<int>(drawHeightHalf_ * 2.f), graph_, WHITE);
	}

	Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
		static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);

	chakram_->DrawChakram();

	Novice::ScreenPrintf(0, 40, "Screenpos %0.2f %0.2f ", screenPos_.x, ConvertToScreen(screenPos_.y));
	Novice::ScreenPrintf(0, 60, "pos %0.2f %0.2f ", pos_.x, pos_.y);
	//Novice::ScreenPrintf(0, 80, "chargeTimer_ %d throw CD %d ", chargeTimer_, throwCD_);
	
	/*Novice::ScreenPrintf(1000, 60, "LB x %d LB y %d ", BlockX(vertex_.leftBottom.x), BlockY(vertex_.leftBottom.y));
	Novice::ScreenPrintf(1000, 200, "scroll X %0.2f Y %0.2f", scrollPos_.x, scrollPos_.y);*/
	Novice::ScreenPrintf(600, 80, "isChakramHit %d", isChakramHit);

	//Novice::ScreenPrintf(600, 80, "can L %d canR  %d", canMoveLeft_, canMoveRight_);
	//Novice::ScreenPrintf(600, 60, "isGround_ %d speed_.y %0.2f ", isGround_, speed_.y);

	Novice::ScreenPrintf(900, 60, "LTx %0.2f LTy %0.2f ", leftTop_.x, leftTop_.y);
	Novice::ScreenPrintf(1130, 60, "RTx %0.2f RTy %0.2f ", rightTop_.x, rightTop_.y);
	Novice::ScreenPrintf(900, 90, "LBx %0.2f LBy %0.2f ", leftBottom_.x, leftBottom_.y);
	Novice::ScreenPrintf(1130, 90, "RBx %0.2f RBy %0.2f ", rightBottom_.x, rightBottom_.y);
}

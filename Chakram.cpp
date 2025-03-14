#include "Chakram.h"
#include "ToScreen.h"
#include "Novice.h"
#include "ToTile.h"

#define _USE_MATH_DEFINES
#include <math.h>

const int kImageWidth = 64;
const int kImageHeight = 64;

Chakram::Chakram() {
	pos_ = { 0.0f, 0.0f };
	stuckPos_ = pos_;
	prePos_ = pos_;
	state_ = NONE;
	maxPos_ = { 0.0f, 0.0f };
	launchPos_ = { 0.0f, 0.0f };

	direction_ = RIGHT;

	screenPos_.x = pos_.x;
	screenPos_.y = ConvertToScreen(pos_.y);

	returnStartPos_ = { 0.0f, 0.0f };

	// timer
	launchTimer_ = 0.0f;
	maxLaunchTimer_ = 30.0f;

	returnTimer_ = 0.0f;
	maxReturnTimer_ = 30.0f;

	spinTimer_ = 0;
	maxSpinTimer_ = 300;

	color_ = WHITE;

	// rotate
	isLaunched_ = false;

	widthHalf_ = 30.f; // HitBox Size
	heightHalf_ = 30.f;
	drawWidthHalf_ = 32.f; // Image Size
	drawHeightHalf_ = 32.f;

	isPressLeft_ = false;
	isPressRight_ = false;
	isPressUp_ = false;
	isPressDown_ = false;

	canMoveLeft_ = false;
	canMoveRight_ = false;
	canMoveUp_ = false;
	canMoveDown_ = false;

	// chakram vertex 
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

	animePosX_ = 0;
	animePosY_ = 0;
	animeFrame_ = 0;

	graph_ = Novice::LoadTexture("./Resources/chakram.png");

	/// target 
	targetPos_.x = 200.0f;
	targetPos_.y = 200.0f;
	aimSpeed_ = { 6.6f, 6.6f };

	

	targetOldPos_ = { 0.0f, 0.0f };
	targetScreenPos_ = { 0.0f, 0.0f };

	targetRadius_ = 16.0f;

	targetWidthHalf_ = targetRadius_; // HitBox Size
	targetHeightHalf_ = targetRadius_;
	targetDrawWidthHalf_ = 16.f; // Image Size
	targetDrawHeightHalf_ = 16.f;

	target_isPressLeft_ = false;
	target_isPressRight_ = false;
	target_isPressUp_ = false;
	target_isPressDown_ = false;

	target_canMoveLeft_ = false;
	target_canMoveRight_ = false;
	target_canMoveUp_ = false;
	target_canMoveDown_ = false;

	// target vertex
	targetLeftTop_.x = targetPos_.x - targetWidthHalf_;
	targetLeftTop_.y = targetPos_.y + targetHeightHalf_;
	targetRightTop_.x = targetPos_.x + targetWidthHalf_ - 1.0f;
	targetRightTop_.y = targetPos_.y + targetHeightHalf_;
	targetLeftBottom_.x = targetPos_.x - targetWidthHalf_;
	targetLeftBottom_.y = targetPos_.y - targetHeightHalf_ + 1.0f;
	targetRightBottom_.x = targetPos_.x + targetWidthHalf_ - 1.0f;
	targetRightBottom_.y = targetPos_.y - targetHeightHalf_ + 1.0f;

	targetLeftTopOld_.x = targetOldPos_.x - targetWidthHalf_;
	targetLeftTopOld_.y = targetOldPos_.y + targetHeightHalf_;
	targetRightTopOld_.x = targetOldPos_.x + targetWidthHalf_ - 1.0f;
	targetRightTopOld_.y = targetOldPos_.y + targetHeightHalf_;
	targetLeftBottomOld_.x = targetOldPos_.x - targetWidthHalf_;
	targetLeftBottomOld_.y = targetOldPos_.y - targetHeightHalf_ + 1.0f;
	targetRightBottomOld_.x = targetOldPos_.x + targetWidthHalf_ - 1.0f;
	targetRightBottomOld_.y = targetOldPos_.y - targetHeightHalf_ + 1.0f;

	crosshairGraph_ = Novice::LoadTexture("./Resources/crosshair.png");
}

void Chakram::SetState(CHAKRAM_STATE state) {
	state_ = state;
}


void Chakram::AimControl(char* keys) {
	// Moving Aim
	target_isPressLeft_ = false;
	target_isPressRight_ = false;
	target_isPressUp_ = false;
	target_isPressDown_ = false;

	if (keys[DIK_LEFT]) {
		targetPos_.x -= aimSpeed_.x;
		target_isPressLeft_ = true;
	} else if (keys[DIK_RIGHT]) {
		targetPos_.x += aimSpeed_.x;
		target_isPressRight_ = true;
	}

	if (keys[DIK_UP]) {
		targetPos_.y += aimSpeed_.y;
		target_isPressUp_ = true;
	} else if (keys[DIK_DOWN]) {
		targetPos_.y -= aimSpeed_.y;
		target_isPressDown_ = true;
	}
}

void Chakram::TargetMapCollision(int(*map)[kMapWidth]) {
	if (map == nullptr) {
		return;
	}

	target_canMoveLeft_ = false;
	target_canMoveRight_ = false;
	target_canMoveUp_ = false;
	target_canMoveDown_ = false;

	// LeftCollision
	if (map[BlockY(targetLeftTopOld_.y)][BlockX(targetLeftTop_.x)] == 0 &&
		map[BlockY(targetLeftBottomOld_.y)][BlockX(targetLeftBottom_.x)] == 0) {
		target_canMoveLeft_ = true;
	} 

	// RightCollision
	if (map[BlockY(targetRightTopOld_.y)][BlockX(targetRightTop_.x)] == 0 &&
		map[BlockY(targetRightBottomOld_.y)][BlockX(targetRightBottom_.x)] == 0) {
		target_canMoveRight_ = true;
	}

	// UpCollision
	if (map[BlockY(targetLeftTop_.y)][BlockX(targetLeftTopOld_.x)] == 0 &&
		map[BlockY(targetRightTop_.y)][BlockX(targetRightTopOld_.x)] == 0) {
		target_canMoveUp_ = true;
	}

	// DownCollision
	if (map[BlockY(targetLeftBottom_.y)][BlockX(targetLeftBottomOld_.x)] == 0 &&
		map[BlockY(targetRightBottom_.y)][BlockX(targetRightBottomOld_.x)] == 0) {
		target_canMoveDown_ = true;
	}
	

	/// Moving 
	if (target_canMoveLeft_ == true) {
		if (target_canMoveUp_ == false || target_canMoveDown_ == false) {
			if (target_isPressLeft_) {
				targetPos_.x = targetOldPos_.x - aimSpeed_.x;
			}
		}

	} else {
		targetPos_.x = static_cast<float>((BlockX(targetLeftTop_.x) + 1) * kBlockSize + targetRadius_);
	}

	if (target_canMoveRight_ == true) {
		if (target_canMoveUp_ == false || target_canMoveDown_ == false) {
			if (target_isPressRight_) {
				targetPos_.x = targetOldPos_.x + aimSpeed_.x;
			}
		}

	} else {
		targetPos_.x = static_cast<float>(BlockX(targetRightBottom_.x) * kBlockSize - targetRadius_);
	}

	if (target_canMoveUp_ == true) {
		if (target_canMoveLeft_ == false || target_canMoveRight_ == false) {
			if (target_isPressUp_) {
				targetPos_.y = targetOldPos_.y + aimSpeed_.y;
			}
		}

	} else {
		targetPos_.y = ConvertToScreen(static_cast<float>((BlockY(targetLeftTop_.y) + 1) * kBlockSize + targetRadius_));
	}

	if (target_canMoveDown_ == true) {
		if (target_canMoveLeft_ == false || target_canMoveRight_ == false) {
			if (target_isPressDown_) {
				targetPos_.y = targetOldPos_.y - aimSpeed_.y;
			}
		}

	} else {
		targetPos_.y = ConvertToScreen(static_cast<float>((BlockY(targetLeftBottom_.y)) * kBlockSize - targetRadius_));
	}

}

void Chakram::ChakramVertexUpdate() {
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

void Chakram::TargetVertexUpdate() {
	targetLeftTop_.x = targetPos_.x - targetWidthHalf_;
	targetLeftTop_.y = targetPos_.y + targetHeightHalf_;
	targetRightTop_.x = targetPos_.x + targetWidthHalf_ - 1.0f;
	targetRightTop_.y = targetPos_.y + targetHeightHalf_;
	targetLeftBottom_.x = targetPos_.x - targetWidthHalf_;
	targetLeftBottom_.y = targetPos_.y - targetHeightHalf_ + 1.0f;
	targetRightBottom_.x = targetPos_.x + targetWidthHalf_ - 1.0f;
	targetRightBottom_.y = targetPos_.y - targetHeightHalf_ + 1.0f;

	targetLeftTopOld_.x = targetOldPos_.x - targetWidthHalf_;
	targetLeftTopOld_.y = targetOldPos_.y + targetHeightHalf_;
	targetRightTopOld_.x = targetOldPos_.x + targetWidthHalf_ - 1.0f;
	targetRightTopOld_.y = targetOldPos_.y + targetHeightHalf_;
	targetLeftBottomOld_.x = targetOldPos_.x - targetWidthHalf_;
	targetLeftBottomOld_.y = targetOldPos_.y - targetHeightHalf_ + 1.0f;
	targetRightBottomOld_.x = targetOldPos_.x + targetWidthHalf_ - 1.0f;
	targetRightBottomOld_.y = targetOldPos_.y - targetHeightHalf_ + 1.0f;
}

void Chakram::SpeedUpSpinTimer(int extraTimer){
	spinTimer_ += extraTimer;
}

void Chakram::Update(char* keys, char* preKeys, Vector2 const& playerPos, Vector2 const& scroll, 
	DIRECTION const& playerDirection, int(*map)[kMapWidth]) {
	if (keys == nullptr || map == nullptr) {
		return;
	}
	if (state_ == NONE) {
		launchTimer_ = 0.0f;
		spinTimer_ = 0;
		isLaunched_ = false;
	}

	if (state_ == PREAIM) {
		targetPos_ = playerPos;
	}

	// targetPos_ = playerPos;
	if (state_ == AIM) {
		targetOldPos_ = targetPos_;
		AimControl(keys);
		TargetVertexUpdate();
		TargetMapCollision(map);

		maxPos_ = targetPos_;

		launchPos_ = playerPos;
		if (playerDirection == RIGHT) {
			direction_ = RIGHT;
		} else {
			direction_ = LEFT;
		}
	}

	if (state_ == LAUNCH) {
		isLaunched_ = true;
		launchTimer_ += 1.0f;
		color_ = WHITE;

		prePos_ = pos_;

		if (launchTimer_ >= maxLaunchTimer_) {
			launchTimer_ = maxLaunchTimer_;
			state_ = SPINNING;
		}

		pos_.x = launchPos_.x + (launchTimer_ / maxLaunchTimer_ * (maxPos_.x - launchPos_.x));
		pos_.y = launchPos_.y + (launchTimer_ / maxLaunchTimer_ * (maxPos_.y - launchPos_.y));
		

	}


	if (state_ == STUCK) {
		returnTimer_ = 0.0f;
		stuckPos_ = pos_;
		returnStartPos_ = stuckPos_;
		if (keys[DIK_V] && !preKeys[DIK_V]) {
			state_ = RETURN;
		}

	} else if (state_ == SPINNING) {
		returnTimer_ = 0.0f;
		returnStartPos_ = maxPos_;
		spinTimer_++;
		if (spinTimer_ >= maxSpinTimer_) {
			state_ = RETURN;
			
		}

		if (keys[DIK_C] && !preKeys[DIK_C]) {
			state_ = RETURN;
		}

		if (spinTimer_ >= maxSpinTimer_ - 120) {
			if (spinTimer_ % 30 < 5 ) {
				color_ = 0xF88379FF;
			} else {
				color_ = WHITE;
			}
		} 
	} 

	

	if (state_ == RETURN) {
		color_ = WHITE;
		returnTimer_++;
		if (returnTimer_ >= maxReturnTimer_) {
	
			returnTimer_ = maxReturnTimer_;
		}

		pos_.x = returnStartPos_.x + (returnTimer_ / maxReturnTimer_ * (playerPos.x - returnStartPos_.x));
		pos_.y = returnStartPos_.y + (returnTimer_ / maxReturnTimer_ * (playerPos.y - returnStartPos_.y));

		if (pos_.x == playerPos.x && pos_.y == playerPos.y) {
			state_ = NONE;
		}
	}
	
	screenPos_.x = pos_.x - scroll.x;
	screenPos_.y = pos_.y - scroll.y;

	targetScreenPos_.x = targetPos_.x - scroll.x;
	targetScreenPos_.y = targetPos_.y - scroll.y;
	AnimeUpdate();
}

void Chakram::AnimeUpdate() {
	if (state_ != NONE && state_ != STUCK) {
		animeFrame_++;
		animePosY_ = 0;
		if (animeFrame_ % 4 == 0) {
			animePosX_ += kImageWidth;

		}
		if (animePosX_ >= kImageWidth * 4) {
			animePosX_ = 0;
		}
	}
}

void Chakram::DrawChakram() {
	
	if (state_ == AIM) {
		Novice::DrawQuad(static_cast<int>(targetScreenPos_.x - targetDrawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(targetScreenPos_.y) - targetDrawHeightHalf_) + 0,
			static_cast<int>(targetScreenPos_.x + targetDrawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(targetScreenPos_.y) - targetDrawHeightHalf_) + 0,
			static_cast<int>(targetScreenPos_.x - targetDrawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(targetScreenPos_.y) + targetDrawHeightHalf_) + 0,
			static_cast<int>(targetScreenPos_.x + targetDrawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(targetScreenPos_.y) + targetDrawHeightHalf_) + 0,
			0, 0, static_cast<int>(targetDrawWidthHalf_ * 2.f), static_cast<int>(targetDrawHeightHalf_ * 2.f), crosshairGraph_, WHITE);
		
		/*Novice::DrawEllipse(static_cast<int>(targetScreenPos_.x), static_cast<int>(ConvertToScreen(targetScreenPos_.y)),
			static_cast<int>(targetRadius_), static_cast<int>(targetRadius_), 0.0f, color_, kFillModeSolid);*/
	}

	if (isLaunched_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, direction_, 0, 0, color_);

		/*Novice::DrawQuad(static_cast<int>(screenPos_.x - drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) - drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x - drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			static_cast<int>(screenPos_.x + drawWidthHalf_) + 0, static_cast<int>(ConvertToScreen(screenPos_.y) + drawHeightHalf_) + 0,
			animePosX_, animePosY_, static_cast<int>(drawWidthHalf_ * 2.f), static_cast<int>(drawHeightHalf_ * 2.f), graph_, WHITE);*/

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/
	}

	/*Novice::ScreenPrintf(0, 500, "ChakramPos X %0.2f Y %0.2f", screenPos_.x, ConvertToScreen(screenPos_.y));
	Novice::ScreenPrintf(0, 520, "TargetPos X %0.2f Y %0.2f", targetScreenPos_.x, ConvertToScreen(targetScreenPos_.y));*/
	//Novice::ScreenPrintf(0, 560, "state_ %d", state_);
	//Novice::ScreenPrintf(0, 580, "spinTimer %d max %d", spinTimer_, maxSpinTimer_);
	//Novice::ScreenPrintf(240, 500, "LaunchPos X %0.2f Y %0.2f", launchPos_.x, launchPos_.y);

}

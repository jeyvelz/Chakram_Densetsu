#include "StageClear.h"

StageClear::StageClear() {
	currentSelection_ = 0;
	arrowPosY_ = 516;
	srcX = 0;

	bgGraph_ = Novice::LoadTexture("./Resources/stageClear/stageClearBG.png");
	textGraph_ = Novice::LoadTexture("./Resources/stageClear/text.png");

	selectingSFX = Novice::LoadAudio("./Resources/audio/select/select3.mp3");
	confirmSFX = Novice::LoadAudio("./Resources/audio/select/select2.mp3");
}

int StageClear::Select(char* keys, char* preKeys) {
	if (keys == nullptr || preKeys == nullptr) {
		return -1;
	}
	if ((keys[DIK_UP] && !preKeys[DIK_UP])) {
		Novice::PlayAudio(selectingSFX, 0, 1.0f);
		currentSelection_--;

		if (currentSelection_ < 0) {
			currentSelection_ = 0;
		}
	}
	if ((keys[DIK_DOWN] && !preKeys[DIK_DOWN])) {
		Novice::PlayAudio(selectingSFX, 0, 1.0f);
		currentSelection_++;

		if (currentSelection_ > 2) {
			currentSelection_ = 2;
		}
	}

	srcX = ((currentSelection_) * 1280);
	if (((keys[DIK_SPACE] && !preKeys[DIK_SPACE]) || (keys[DIK_Z] && !preKeys[DIK_Z]))) {
		Novice::PlayAudio(confirmSFX, 0, 1.0f);

		switch (currentSelection_) {
		case 0:

			return 1;
			break;
		case 1:

			return 2;
			break;
		case 2:

			return 3;
			break;
		}

	}
	return 0;
}


void StageClear::Draw() {
	Novice::DrawSprite(0, 0, bgGraph_, 1.0f, 1.0f, 0.0f, WHITE);

	Novice::DrawSpriteRect(0, 0, srcX, 0, 1280, 720, textGraph_, 1.0f / 3.0f, 1.0f, 0.0f, WHITE);

	// text

	//Novice::ScreenPrintf(100, 0, "Current : %d", currentSelection_);
}

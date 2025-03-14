#include "StageSelect.h"


StageSelect::StageSelect() {
	currentSelection_ = 0;
	textColor_ = WHITE;

	srcX = 0;

	bgGraph_ = Novice::LoadTexture("./Resources/stageSelect/stageSelectBG.png");
	stagesGraph_ = Novice::LoadTexture("./Resources/stageSelect/stages.png");

	selectSFX_ = Novice::LoadAudio("./Resources/audio/select/select3.mp3");

	for (int i = 0; i < 4; i++) {
		scrollPos_[i] = 0.0f;
	}
}

int StageSelect::Select(char* keys, char* preKeys) {
	if (keys == nullptr || preKeys == nullptr) {
		return -1;
	}
	if ((keys[DIK_LEFT] && !preKeys[DIK_LEFT])) {
		Novice::PlayAudio(selectSFX_, 0, 1.0f);
		currentSelection_--;

		if (currentSelection_ < 0) {
			currentSelection_ = 0;
		}
	}
	if ((keys[DIK_RIGHT] && !preKeys[DIK_RIGHT])) {
		Novice::PlayAudio(selectSFX_, 0, 1.0f);
		currentSelection_++;

		if (currentSelection_ > 3) {
			currentSelection_ = 3;
		}
	}

	srcX = ((currentSelection_) * 1280);
	
	switch (currentSelection_) {
	case 0:

		return 0;
		break;
	case 1:

		return 1;
		break;
	case 2:

		return 2;
		break;
	case 3:

		return 3;
		break;
	}

	
	return 0;
}


void StageSelect::Draw() {
	Novice::DrawSprite(0, 0, bgGraph_, 1.0f, 1.0f, 0.0f, WHITE);

	Novice::DrawSpriteRect(0, 0, srcX, 0, 1280, 720, stagesGraph_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
	
	//Novice::ScreenPrintf(100, 0, "Current : %d", currentSelection_);
}


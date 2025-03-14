#include "Title.h"

Title::Title() {
	currentSelection_ = 0;
	textColor_ = WHITE;

	srcX = 0;

	bgGraph_ = Novice::LoadTexture("./Resources/title/title.png");
	textGraph_ = Novice::LoadTexture("./Resources/title/text.png");
	chakramGraph_ = Novice::LoadTexture("./Resources/title/chakramx4.png");

	animeFrame = 0;
	animeX = 0;
	animeY = 0;

	selectingSFX = Novice::LoadAudio("./Resources/audio/select/select3.mp3");
	confirmSFX = Novice::LoadAudio("./Resources/audio/select/select2.mp3");

	for (int i = 0; i < 3; i++) {
		scrollPos_[i] = 0.0f;
	}
}

int Title::Select(char* keys, char* preKeys) {
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

		if (currentSelection_ > 1) {
			currentSelection_ = 1;
		}
	}

	srcX = ((currentSelection_) * 1280);
	if (((keys[DIK_SPACE] && !preKeys[DIK_SPACE]) || (keys[DIK_Z] && !preKeys[DIK_Z]))) {
		Novice::PlayAudio(confirmSFX, 0, 1.0f);

		switch (currentSelection_) {
		case 0://スタート

			return 1;
			break;
		case 1://フールスクリーン

			return 2;
			break;
		}
	
	}
	return 0;
}


void Title::Draw() {
	// bg
	Novice::DrawSprite(0, 0, bgGraph_, 1.0f, 1.0f, 0.0f, WHITE);

	animeFrame++;
	animeY = 0;
	if (animeFrame % 6 == 0) {
		animeX += 128;

	}
	if (animeX >= 128 * 4) {
		animeX = 0;
		animeFrame = 0;
	}
	Novice::DrawSpriteRect(370, 220, animeX, animeY, 128, 128, chakramGraph_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);

	// text
	Novice::DrawSpriteRect(0, 0, srcX, 0, 1280, 720, textGraph_, 1.0f / 2.0f, 1.0f, 0.0f, WHITE);
	
	//Novice::ScreenPrintf(100, 0, "Current : %d", currentSelection_);
}

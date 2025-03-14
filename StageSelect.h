#pragma once
#include "Novice.h"

class StageSelect {
private:
	int currentSelection_ = 0;
	//bool isWindowMode_ = true;
	unsigned int textColor_;
	float scrollPos_[4];

	int srcX;

	int bgGraph_;
	int stagesGraph_;

	int selectSFX_;

public:
	StageSelect();
	int Select(char* keys, char* preKeys);
	void Draw();

	int GetStage() { return currentSelection_; }
};



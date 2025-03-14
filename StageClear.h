#pragma once
#include "Novice.h"

class StageClear {
private:
	int currentSelection_ = 0;

	int arrowPosY_;

	int bgGraph_;
	int textGraph_;

	int selectingSFX;
	int confirmSFX;

	int srcX;

public:
	StageClear();
	int Select(char* keys, char* preKeys);
	void Draw();
};


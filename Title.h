#pragma once
#include "Novice.h"

class Title {
private:
	int currentSelection_ = 0;
	//bool isWindowMode_ = true;
	unsigned int textColor_;
	float scrollPos_[3];

	int bgGraph_;
	int textGraph_;

	int chakramGraph_;
	int animeFrame;
	int animeX;
	int animeY;

	int selectingSFX;
	int confirmSFX;
	
	int srcX;

public:
	Title();
	int Select(char* keys, char* preKeys);
	void Draw();
};


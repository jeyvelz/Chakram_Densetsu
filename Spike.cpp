#include "Spike.h"
#include "ToScreen.h"

Spike::Spike() {
	spawnPos_ = { 0.0f, 0.0f };
	pos_ = { 1360.0f, 400.0f };
	screenPos_ = pos_;

	widthHalf_ = 32.f;
	heightHalf_ = 16.f;

	drawWidthHalf_ = 32.f;
	drawHeightHalf_ = 16.f;

	isSpawn_ = false;
	state_ = ENEMY_IDLE;

	graph_ = Novice::LoadTexture("./Resources/enemy/spikes.png");
}


void Spike::Update(const Player* player) {
	if (player == nullptr) {
		return;
	}

	screenPos_.x = pos_.x - player->GetScrollPosX();
	screenPos_.y = pos_.y - player->GetScrollPosY();
}

void Spike::SpawnSpike(Vector2 pos) {
	spawnPos_ = pos;
	pos_.x = (spawnPos_.x + 1.0f) * kBlockSize - (kBlockSize / 2);
	pos_.y = spawnPos_.y * kBlockSize + heightHalf_;
	isSpawn_ = true;
	state_ = ENEMY_IDLE;
}

void Spike::DrawSpike() {
	if (isSpawn_ == true) {
		DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, 0, 0, graph_, RIGHT, 0, 0, color_);

		/*Novice::DrawBox(static_cast<int>(screenPos_.x - widthHalf_), static_cast<int>(ConvertToScreen(screenPos_.y) + heightHalf_),
			static_cast<int>(widthHalf_ * 2.f), static_cast<int>(-(heightHalf_ * 2.f)), 0.f, RED, kFillModeWireFrame);*/
	}

}
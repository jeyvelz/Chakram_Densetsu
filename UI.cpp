#include "UI.h"
#include <Novice.h>

PlayerUI::PlayerUI() {
	iconGraph_ = Novice::LoadTexture("./Resources/UI/icon.png");
	hpGraph_ = Novice::LoadTexture("./Resources/UI/life.png");
}

void PlayerUI::DrawPlayerUI(int const& hp) {
	Novice::DrawSprite(0, 0, iconGraph_, 1.0f, 1.0f, 0.0f, WHITE);
	if (hp > 0) {
		for (int i = 0; i < hp; i++) {
			Novice::DrawSprite(80 + (i * 48), 5, hpGraph_, 1.f, 1.f, 0.f, WHITE);
		}
	}
}

BossUI::BossUI() {
	bossIconGraph_ = Novice::LoadTexture("./Resources/UI/bossIcon.png");
	bossHpBarGraph_ = Novice::LoadTexture("./Resources/UI/bossHealthBar.png");
	footHpBarGraph_ = Novice::LoadTexture("./Resources/UI/bossFootHpBar.png");
}

void BossUI::DrawBossUI(int const& hp, int const& maxHp, bool const& isStunned,
int const& LFootHp, int const& RFootHp) {
	Novice::DrawBox(744, 720 - 64 + 2, 404, 64 - 6, 0.0f, 0x5e5e5eFF, kFillModeSolid);
	int width = -static_cast<int>(static_cast<float>(hp) / static_cast<float>(maxHp) * 400);
	if (hp > 0) {
		Novice::DrawBox(1148, 720 - 64 + 2, width, 64 - 6, 0.0f, RED, kFillModeSolid);
	}
	Novice::DrawSprite(1280 - 408 - 128, 720 - 64, bossHpBarGraph_, 1.0f, 1.0f, 0.0f, WHITE);
	// icon
	if (isStunned) {
		Novice::DrawSpriteRect(1280 - 128, 720 - 128, 128, 0, 128, 128, bossIconGraph_, 1.0f / 2.0f, 1.0f, 0.0f, WHITE);
	} else {
		Novice::DrawBox(747, 720 - 60, 402, 64 - 10, 0.0f, 0x454b54CF, kFillModeSolid);
		Novice::DrawSpriteRect(1280 - 128, 720 - 128, 0, 0, 128, 128, bossIconGraph_, 1.0f / 2.0f, 1.0f, 0.0f, WHITE);
	}
	 
	// foot
	Novice::DrawBox(1280 - 128 - 128, 720 - 64 - 25, 128, 22, 0.0f, 0x5e5e5eFF, kFillModeSolid);
	int leftFootHp = -static_cast<int>(static_cast<float>(LFootHp) / 30 * 60);
	if (LFootHp > 0) {
		Novice::DrawBox(1150 - 64, 720 - 88, leftFootHp, 20, 0.0f, RED, kFillModeSolid);
	}
	int rightFootHp = -static_cast<int>(static_cast<float>(RFootHp) / 30 * 60);
	if (RFootHp > 0) {
		Novice::DrawBox(1150, 720 - 88, rightFootHp, 20, 0.0f, RED, kFillModeSolid);
	}

	Novice::DrawSprite(1280 - 128 - 128, 720 - 64 - 50, footHpBarGraph_, 1.0f, 1.0f, 0.0f, WHITE);

}
#include "Enemy.h"
#include "Novice.h"

Enemy::Enemy() {
	spawnPos_ = { 0.0f, 0.0f };
	state_ = ENEMY_IDLE;

	isSpawn_ = false;

	imageWidth_ = 64;
	imageHeight_ = 64;

	isInvincible_ = false;
	invincibleTimer_ = 0;

	color_ = WHITE;

	graph_ = Novice::LoadTexture("./Resources/enemy/slime.png");
}

bool Enemy::WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) {
	float chakramPosX = chakram->GetPosX();
	float chakramPosY = chakram->GetPosY();

	float chakramWidthHalf = chakram->GetWidthHalf();
	float chakramHeightHalf = chakram->GetHeightHalf();


	if (enemyPos.x - enemyWidthHalf < chakramPosX + chakramWidthHalf && enemyPos.x + enemyWidthHalf > chakramPosX - chakramWidthHalf) {
		if (enemyPos.y - enemyHeightHalf < chakramPosY + chakramHeightHalf && enemyPos.y + enemyHeightHalf > chakramPosY - chakramHeightHalf) {

			return true;
		}
	}
			
	return false;
}

void Enemy::Update(const Player* player) {
	if (player == nullptr) {
		return;
	}

	screenPos_.x = pos_.x - player->GetScrollPosX();
	screenPos_.y = pos_.y - player->GetScrollPosY();
}

void Enemy::SetState(ENEMY_STATE state) {
	state_ = state;
}

void Enemy::AnimeUpdate() {

}

void Enemy::DrawEnemy() {

	DrawObject(screenPos_, drawWidthHalf_, drawHeightHalf_, animePosX_, animePosY_, graph_, direction_, 0, 0);
}
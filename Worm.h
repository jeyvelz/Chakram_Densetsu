#pragma once
#include "Enemy.h"
#include "WormBullet.h"

class Worm : public Enemy {
public:
	WormBullet* bullet_;
	Worm();

	~Worm() = default;

	void Attack();

	void Update(const Player* player);

	void AnimeUpdate() override;

	void DrawEnemy() override;

	void SpawnWorm(Vector2 pos);

	bool WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) override;
	bool SlashCollision(PlayerAttack* slash);

	void SetState(ENEMY_STATE state);
	ENEMY_STATE GetState() { return state_; }

	void GiveDamage(int damage);

	void SetPos(Vector2 pos);
	Vector2 GetPos() { return pos_; }
	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

private:
	int maxInvincibleTimer_;
	int atkTimer_;
	int atkTiming_;
};


#pragma once
#include "Enemy.h"

class FireSpirit : public Enemy {
public:
	FireSpirit();

	~FireSpirit() = default;

	void Move(const Player* player);

	void Update(const Player* player) override;

	void AnimeUpdate() override;

	void DrawEnemy() override;

	void SpawnFireSpirit(Vector2 pos, float attackDistance);

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
	int deadFrame_;

	float attackDistance_;
};


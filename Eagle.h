#pragma once
#include "Enemy.h"

class Eagle : public Enemy {
public:
	Eagle();

	~Eagle() = default;

	void Move();

	void Update(const Player* player) override;

	void AnimeUpdate() override;

	void DrawEnemy() override;

	void SpawnEagle(Vector2 pos, float movementRange);

	bool WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) override;

	void SetState(ENEMY_STATE state);
	ENEMY_STATE GetState() { return state_; }

	void GiveDamage(int damage);

	void SetPos(Vector2 pos);
	Vector2 GetPos() { return pos_; }
	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

private:
	float movementRangeY_;
};



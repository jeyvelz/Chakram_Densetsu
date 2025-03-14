#pragma once
#include "Enemy.h"

class Slime: public Enemy {
public:
	Slime();

	~Slime() = default;

	void Move();

	void MapCollision(int(*map)[kMapWidth]);

	void Update(const Player* player, int(*map)[kMapWidth], Vector2 slashPos);

	void VertexUpdate();

	void AnimeUpdate() override;

	void DrawEnemy() override;

	/// <summary>
	/// Function to spawn Slime
	/// </summary>
	/// <param name="pos">Set Spawn Position</param>
	void SpawnSlime(Vector2 pos);

	bool WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf) override;

	void Damaged(Vector2 slashPos);
	/// <summary>
	/// Player Melee/Slash Atk Collision Function
	/// </summary>
	/// <param name="slash">Slash Attack</param>
	/// <returns></returns>
	bool SlashCollision(PlayerAttack* slash);

	/// <summary>
	/// Give Damage to Enemy
	/// </summary>
	/// <param name="damage">Set Damage</param>
	void GiveDamage(int damage);
	
	/// Getter and Setter
	void SetState(ENEMY_STATE state);
	ENEMY_STATE GetState() { return state_; }

	void SetPos(Vector2 pos);
	Vector2 GetPos() { return pos_; }
	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

private:
	int maxInvincibleTimer_;
	Vector2 knockback_;
	bool isGround_;
};


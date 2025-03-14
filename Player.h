#pragma once
#include "define.h"
#include "Object.h"
#include "Chakram.h"
#include "PlayerAttack.h"


enum PLAYER_STATE {
	IDLE,
	WALK,
	JUMP,
	FALL,
	ATTACK,
	CHARGE,
	THROW_ATTACK,
	DAMAGED
};


class Player: public Object{
public:
	Chakram* chakram_;
	PlayerAttack* slash_;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	void MapCollisionDetection(int(*map)[kMapWidth]);

	void HorizontalMove(char* keys);

	void Move(char* keys, char* preKeys, int(*map)[kMapWidth]);

	/// <summary>
	///	BoxCollision
	/// </summary>
	/// <param name="pos">Enemy Pos</param>
	/// <param name="widthHalf">Enemy Hit Box Width</param>
	/// <param name="heightHalf">Enemy Hit Box Height</param>
	/// <returns>Collide or Not</returns>
	bool BoxCollision(Vector2 const& pos, float const& widthHalf, float const& heightHalf);

	void Draw();

	/// Setter Getter
	Vector2 GetScrollPos() const { return scrollPos_; }
	float GetScrollPosX() const { return scrollPos_.x; }
	float GetScrollPosY() const { return scrollPos_.y; }

	Vector2 GetPos() { return pos_; }
	float GetPosX() const { return pos_.x; }
	float GetPosY() const { return pos_.y; }

	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

	int GetHP() { return hp_; }

	void Damaged(int damage);
	
	void SpawnPlayer(Vector2 pos);

	void DespawnPlayer();

private:
	bool isOpen = false;
	void Scroll();

	void VertexUpdate();

	void AnimeUpdate();

private:
	bool isSpawn_;
	int noChakramGraph_;
	int chakramGraph_;

	Vector2 scrollPos_;
	bool isPressLeft_;
	bool isPressRight_;

	bool canMoveLeft_;
	bool canMoveRight_;

	bool isGround_;
	bool isOnPlatform_;
	bool isAtk_;

	float jumpHeight_;

	int atkTimer_;
	int maxAtkTimer_;
	int atkHit_;

	int maxAtkFrame_;
	int atkFrame_;

	int chargeTimer_;
	int throwCD_;

	PLAYER_STATE state_;

	Vector2 knockback_;
	Vector2 atkMovement_;
	bool isHit = false;

	int invincibleFrame_;
	int kInvincibleFrame_;

	bool isTopCollision_;

	// audio
	int damagedSFX;
	int swing1SFX;
	int swing2SFX;
	int jumpSFX;
};


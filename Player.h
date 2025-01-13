#pragma once
#include "define.h"
#include "Object.h"
#include "Chakram.h"

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

	bool BoxCollision(Vector2 const& pos, float const& widthHalf, float const& heightHalf);

	void Draw();

	/// Setter Getter
	Vector2 GetScrollPos() { return scrollPos_; }
private:
	
	void Scroll();

	void VertexUpdate();

	void AnimeUpdate();

private:
	bool isPressLeft_;
	bool isPressRight_;

	bool canMoveLeft_;
	bool canMoveRight_;

	bool isGround_;
	bool isAtk_;

	float jumpHeight_;

	int chargeTimer_;
	int throwCD_;

	PLAYER_STATE state_;

	bool isChakramHit;
};


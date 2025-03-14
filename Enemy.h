#pragma once
#include "define.h"
#include "Object.h"
#include "Player.h"
#include "Chakram.h"

enum ENEMY_STATE {
	ENEMY_IDLE,
	ENEMY_MOVE,
	ENEMY_ATTACK,
	ENEMY_DAMAGED,
	ENEMY_DEAD
};

class Enemy:public Object{
public:

	Enemy();

	virtual ~Enemy() = default;

	virtual bool WeaponCollision(Chakram* chakram, Vector2 enemyPos, float enemyWidthHalf, float enemyHeightHalf);

	virtual void Update(const Player* player);

	virtual void AnimeUpdate();

	virtual void DrawEnemy();

	void SetState(ENEMY_STATE state);
	ENEMY_STATE GetState() { return state_; }

protected:
	Vector2 spawnPos_;

	int invincibleTimer_;
	int isInvincible_;

	int color_;
public:
	bool isSpawn_;

	ENEMY_STATE state_;
	// for animation
	int imageWidth_;
	int imageHeight_;
};


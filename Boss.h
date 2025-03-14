#pragma once
#include "Object.h"
#include "Player.h"
#include "Rock.h"
#include "Fireball.h"

enum BOSS_STATE {
	BOSS_IDLE,
	BOSS_STUN,
	BOSS_RIGHT_ATK,
	BOSS_LEFT_ATK,
	BOSS_ROCK,
	BOSS_FIREBALL,
	BOSS_DEAD
};

class Boss: public Object{
public:
	Rock* rocks;
	Fireball* fireballs;

	Boss();

	~Boss();

	void Update(const Player* player);

	void Draw(int shakeX, int shakeY)const;

	bool ChakramCollision(Chakram* chakram);

	bool SlashCollision(PlayerAttack* slash);

	bool FootChakramCollision(Chakram* chakram);

	bool FootSlashCollision(PlayerAttack* slash);

	int GetHP() { return hp_;}

	void SpawnBoss();

	bool GetStunned() { return isStunned_; }

private:
	void AtkManager();

	void LeftAtk();
	void RightAtk();

	//void FireballAtk();

private:
	
	int isSpawn_;
	int atkTimer_;
	
	bool isStunned_;
	int isInvincible_;
	int invincibleTimer_;
	unsigned int color_;

	struct Parts {
		int hp_;
		Vector2 startPos_;
		Vector2 pos_;
		Vector2 leftTop_;
		Vector2 rightTop_;
		Vector2 leftBottom_;
		Vector2 rightBottom_;

		Vector2 screenPos_;

		Vector2 speed_;

		float widthHalf_;
		float heightHalf_;

		float drawWidthHalf_;
		float drawHeightHalf_;

		DIRECTION partsDirection_;
		unsigned int color_;
	};

	bool isPartInvincible_;
	int partInvincibleTimer_;
	int maxPartInvincibleTimer_;

	Parts left;
	Parts right;

	Parts stomp;
	Parts gem;
	int maxInvincibleTimer_;
	
	int stompTimer_;
	int stompCounter_;

	int fireballTimer_;

	int loop_;

	// image graph
	int bodyGraph_;
	int leftFootGraph_;
	int rightFootGraph_;
	int breakGraph_;

	int stompGraph_;
	int gemGraph_;
	int shineGraph_;

	// audio
	int footSpinDamagedSFX_;
	int footSlashDamagedSFX_;
	int footDestroyedSFX_;

	int stompSFX_;

public:
	// stomp
	Vector2 GetStompPos() { return stomp.pos_; }
	float GetStompWidthHalf() { return stomp.widthHalf_; }
	float GetStompHeightHalf() { return stomp.heightHalf_; }

	// Foot
	Vector2 GetLeftFootPos() { return left.pos_; }
	Vector2 GetRightFootPos() { return right.pos_; }

	float GetFootWidthHalf() { return left.widthHalf_; }
	float GetFootHeightHalf() { return left.heightHalf_; }

	void SetLeftFootHP(int damage) {
		left.hp_ -= damage;
	}

	void SetRightFootHP(int damage) {
		right.hp_ -= damage;
	}

	int GetLeftFootHP() { return left.hp_; }
	int GetRightFootHP() { return right.hp_; }

	BOSS_STATE state_;
	bool isStomp_;
};


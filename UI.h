#pragma once
#include "Player.h"

class PlayerUI {
public:
	PlayerUI();
	~PlayerUI() = default;
	
	void DrawPlayerUI(int const& hp);
private:
	int iconGraph_;
	int hpGraph_;
	//int chakramGraph_;
};

class BossUI {
public:
	BossUI();
	~BossUI() = default;

	void DrawBossUI(int const& hp, int const& maxHp, bool const& isStunned, 
		int const& LFootHp, int const& RFootHp);

private:
	int bossIconGraph_;
	int bossHpBarGraph_;
	int footHpBarGraph_;
};
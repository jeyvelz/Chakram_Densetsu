#pragma once
#include "Enemy.h"

class Spike: public Enemy {
public:
	Spike();

	~Spike() = default;

	void Update(const Player* player);

	void DrawSpike() ;

	void SpawnSpike(Vector2 pos);

	Vector2 GetPos() { return pos_; }
	float GetWidthHalf() { return widthHalf_; }
	float GetHeightHalf() { return heightHalf_; }

};


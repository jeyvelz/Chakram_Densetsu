#pragma once
#include "Object.h"
#define kMaxFireball 6

struct Fire {
	Vector2 pos_;
	Vector2 screenPos_;
	float widthHalf_;
	float heightHalf_;
	float speedX_;
	bool isVisible_;
	DIRECTION direction_;
};

class Fireball:public Object{
public:
	Fireball();
	~Fireball() = default;

	void Reset();

	void Update(const Vector2 scroll);

	void Draw();

	bool Collision(const Vector2 pos, float widthHalf, float heightHalf);
private:
	Fire fireballs[kMaxFireball];

	int timer_;
public:
	bool isLaunched_;
	int cooldown_;
};


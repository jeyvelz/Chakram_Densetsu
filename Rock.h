#pragma once
#include "Object.h"
#define kMaxRocks 42

struct Rubble {
	Vector2 pos_; 
	Vector2 screenPos_; 
	float widthHalf_;
	float heightHalf_;
	float speedY_;
	bool isVisible_;
	DIRECTION direction_;
};

class Rock : public Object{
public: 
	Rock();
	~Rock() = default;

	void Update(const Vector2 scroll);
	
	void Draw();

	bool Collision(const Vector2 pos, float widthHalf, float heightHalf);


private:
	Rubble rocks[kMaxRocks];

	int timer_;

	int particleCount_;

	int spawnWidth_;
	int spawnHeight_;

	// warning sign
	Vector2 warningPos;
	Vector2 warningScreenPos;
	
	int warningGraph_;
public:
	bool isLaunched_;
};


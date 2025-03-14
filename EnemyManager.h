#pragma once
#include "Slime.h"
#include "Eagle.h"
#include "Worm.h"
#include "FireSpirit.h"
#include "Spike.h"
#include <vector>


class EnemyManager{
public:
	EnemyManager();
	~EnemyManager();

	/// <summary>
	/// Set Max Enemies can spawn per stages
	/// </summary>
	/// <param name="slimeMax">Slime Max Amount</param>
	/// <param name="eagleMax">Eagle Max Amount</param>
	/// <param name="wormMax">Worm Max Amount</param>
	void SetMaxEnemies(int slimeMax, int eagleMax, int wormMax, int fireSpiritMax, int spikeMax);

	//void SpawnEnemy(ENEMY_TYPE enemyType, Vector2 pos, float eagleMovementRange);

	void SpawnSlime(Vector2 pos);
	void SpawnEagle(Vector2 pos, float eagleMovementRange);
	void SpawnWorm(Vector2 pos);
	void SpawnFireSpirit(Vector2 pos, float attackDistance);
	void SpawnSpike(Vector2 pos);

	void DespawnEnemies();

	void UpdateEnemy(const Player* player, int(*map)[kMapWidth], Vector2 slashPos);

	void DrawEnemy();

	void CheckCollision(Player* player);

	void WeaponCollision(Chakram* chakram, PlayerAttack* slash);

private:

	int kMaxSlime;
	int kMaxEagle;
	int kMaxWorm;
	int kMaxFireSpirit;
	int kMaxSpike;

	Slime* slimes[10];  // assuming a higher max for all types
	Eagle* eagles[10];
	Worm* worms[10];
	FireSpirit* fireSpirits[10];
	Spike* spikes[20];

	// audio
	int damagedSFX;
	int spinDamagedSFX;
};


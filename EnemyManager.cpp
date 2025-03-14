#include "EnemyManager.h"
#include "Novice.h"

EnemyManager::EnemyManager() {
	kMaxSlime = 6;
	kMaxEagle = 5;
	kMaxWorm = 5;
	kMaxFireSpirit = 8;
	kMaxSpike = 20;

	for (int i = 0; i < kMaxSlime; i++) {
		slimes[i] = new Slime();
	}

	for (int i = 0; i < kMaxEagle; i++) {
		eagles[i] = new Eagle();
	}

	for (int i = 0; i < kMaxWorm; i++) {
		worms[i] = new Worm();
	}

	for (int i = 0; i < kMaxFireSpirit; i++) {
		fireSpirits[i] = new FireSpirit();
	}

	for (int i = 0; i < kMaxSpike; i++) {
		spikes[i] = new Spike();
	}

	damagedSFX = Novice::LoadAudio("./Resources/audio/sfx/enemyDamaged.mp3");
	spinDamagedSFX = Novice::LoadAudio("./Resources/audio/sfx/spinDamaged.mp3");
}

EnemyManager::~EnemyManager() {
	for (int i = 0; i < kMaxSlime; i++) {
		delete slimes[i];
	}

	for (int i = 0; i < kMaxEagle; i++) {
		delete eagles[i];
	}

	for (int i = 0; i < kMaxWorm; i++) {
		delete worms[i];
	}

	for (int i = 0; i < kMaxFireSpirit; i++) {
		delete fireSpirits[i];
	}

	for (int i = 0; i < kMaxSpike; i++) {
		delete spikes[i];
	}
}

void EnemyManager::SetMaxEnemies(int slimeMax, int eagleMax, int wormMax, int fireSpiritMax, int spikeMax) {
	kMaxSlime = slimeMax;
	kMaxEagle = eagleMax;
	kMaxWorm = wormMax;
	kMaxFireSpirit = fireSpiritMax;
	kMaxSpike = spikeMax;
}

	

void EnemyManager::SpawnSlime(Vector2 pos) {
	for (int i = 0; i < kMaxSlime; i++) {
		if (slimes[i]->isSpawn_ == false) {
			slimes[i]->SpawnSlime(pos);
			break;
		}
	}
}

void EnemyManager::SpawnEagle(Vector2 pos, float eagleMovementRange) {
	for (int i = 0; i < kMaxEagle; i++) {
		if (eagles[i]->isSpawn_ == false) {
			eagles[i]->SpawnEagle(pos, eagleMovementRange);
			break;
		}
	}
}

void EnemyManager::SpawnWorm(Vector2 pos) {
	for (int i = 0; i < kMaxWorm; i++) {
		if (worms[i]->isSpawn_ == false) {
			worms[i]->SpawnWorm(pos);
			break;
		}
	}
}

void EnemyManager::SpawnFireSpirit(Vector2 pos, float attackDistance) {
	for (int i = 0; i < kMaxFireSpirit; i++) {
		if (fireSpirits[i]->isSpawn_ == false) {
			fireSpirits[i]->SpawnFireSpirit(pos, attackDistance);
			break;
		}
	}
}

void EnemyManager::SpawnSpike(Vector2 pos) {
	for (int i = 0; i < kMaxSpike; i++) {
		if (spikes[i]->isSpawn_ == false) {
			spikes[i]->SpawnSpike(pos);
			break;
		}
	}
}

void EnemyManager::DespawnEnemies() {
	for (int i = 0; i < kMaxSlime; i++) {
		if (slimes[i]->isSpawn_ == true) {
			slimes[i]->isSpawn_ = false;
			break;
		}
	}
	for (int i = 0; i < kMaxEagle; i++) {
		if (eagles[i]->isSpawn_ == true) {
			eagles[i]->isSpawn_ = false;
			break;
		}
	}
	for (int i = 0; i < kMaxWorm; i++) {
		if (worms[i]->isSpawn_ == true) {
			worms[i]->isSpawn_ = false;
			break;
		}
	}
	for (int i = 0; i < kMaxFireSpirit; i++) {
		if (fireSpirits[i]->isSpawn_ == true) {
			fireSpirits[i]->isSpawn_ = false;
			break;
		}
	}
	for (int i = 0; i < kMaxSpike; i++) {
		if (spikes[i]->isSpawn_ == true) {
			spikes[i]->isSpawn_ = false;
		}
	}
}

void EnemyManager::UpdateEnemy(const Player* player, int(*map)[kMapWidth], Vector2 slashPos) {
	for (int i = 0; i < kMaxSlime; i++) {
		if (slimes[i]->isSpawn_ == true) {
			slimes[i]->Update(player, map, slashPos);
		}
	}

	for (int i = 0; i < kMaxEagle; i++) {
		if (eagles[i]->isSpawn_ == true) {
			eagles[i]->Update(player);
		}
	}

	for (int i = 0; i < kMaxWorm; i++) {
		if (worms[i]->isSpawn_ == true) {
			worms[i]->Update(player);
		}
	}

	for (int i = 0; i < kMaxFireSpirit; i++) {
		if (fireSpirits[i]->isSpawn_ == true) {
			fireSpirits[i]->Update(player);
		}
	}

	for (int i = 0; i < kMaxSpike; i++) {
		if (spikes[i]->isSpawn_ == true) {
			spikes[i]->Update(player);
		}
	}
}

void EnemyManager::CheckCollision(Player* player) {
	for (int i = 0; i < kMaxSlime; i++) {
		if (slimes[i]->GetState() != ENEMY_DEAD) {
			if (player->BoxCollision(slimes[i]->GetPos(), slimes[i]->GetWidthHalf(), slimes[i]->GetHeightHalf())) {
				player->Damaged(1);
			}
		}
	}

	for (int i = 0; i < kMaxEagle; i++) {
		if (eagles[i]->GetState() != ENEMY_DEAD) {
			if (player->BoxCollision(eagles[i]->GetPos(), eagles[i]->GetWidthHalf(), eagles[i]->GetHeightHalf())) {
				player->Damaged(1);
			} 
		}
	}

	for (int i = 0; i < kMaxWorm; i++) {
		if (worms[i]->GetState() != ENEMY_DEAD) {
			if (player->BoxCollision(worms[i]->GetPos(), worms[i]->GetWidthHalf(), worms[i]->GetHeightHalf())) {
				player->Damaged(1);
			}
			if (player->BoxCollision(worms[i]->bullet_->GetPos(), worms[i]->bullet_->GetWidthHalf(), worms[i]->bullet_->GetHeightHalf())) {
				player->Damaged(1);
			}
		}
	}

	for (int i = 0; i < kMaxFireSpirit; i++) {
		if (fireSpirits[i]->GetState() != ENEMY_DEAD) {
			if (player->BoxCollision(fireSpirits[i]->GetPos(), fireSpirits[i]->GetWidthHalf(), fireSpirits[i]->GetHeightHalf())) {
				player->Damaged(1);
			}
		}
	}

	for (int i = 0; i < kMaxSpike; i++) {
		if (spikes[i]->isSpawn_ == true) {
			if (player->BoxCollision(spikes[i]->GetPos(), spikes[i]->GetWidthHalf(), spikes[i]->GetHeightHalf())) {
				player->Damaged(1);
			}
		}
	}
}

void EnemyManager::WeaponCollision(Chakram* chakram, PlayerAttack* slash) {
	for (int i = 0; i < kMaxSlime; i++) { // slime HP = 4
		if (slimes[i]->WeaponCollision(chakram, slimes[i]->GetPos(), slimes[i]->GetWidthHalf(), slimes[i]->GetHeightHalf())) {
			Novice::PlayAudio(spinDamagedSFX, 0, 1.0f);
			slimes[i]->GiveDamage(1);
			chakram->SpeedUpSpinTimer(30);
		} 
		if (slimes[i]->SlashCollision(slash)) {
			Novice::PlayAudio(damagedSFX, 0, 1.0f);
			slimes[i]->GiveDamage(5);
		}
	}

	for (int i = 0; i < kMaxEagle; i++) { // eagle HP = 3
		if (eagles[i]->WeaponCollision(chakram, eagles[i]->GetPos(), eagles[i]->GetWidthHalf(), eagles[i]->GetHeightHalf())) {
			Novice::PlayAudio(spinDamagedSFX, 0, 1.0f);
			eagles[i]->GiveDamage(1);
			chakram->SpeedUpSpinTimer(30);
		}
	}

	for (int i = 0; i < kMaxWorm; i++) { // worm HP = 5
		if (worms[i]->WeaponCollision(chakram, worms[i]->GetPos(), worms[i]->GetWidthHalf(), worms[i]->GetHeightHalf())) {
			Novice::PlayAudio(spinDamagedSFX, 0, 1.0f);
			worms[i]->GiveDamage(1);
			chakram->SpeedUpSpinTimer(30);
		}
		if (worms[i]->SlashCollision(slash)) {
			Novice::PlayAudio(damagedSFX, 0, 1.0f);
			worms[i]->GiveDamage(4);
		}
	}

	for (int i = 0; i < kMaxFireSpirit; i++) { // fire spirit HP = 3
		if(fireSpirits[i]->WeaponCollision(chakram, fireSpirits[i]->GetPos(), fireSpirits[i]->GetWidthHalf(), fireSpirits[i]->GetHeightHalf())) {
			Novice::PlayAudio(spinDamagedSFX, 0, 1.0f);
			fireSpirits[i]->GiveDamage(1);
			chakram->SpeedUpSpinTimer(30);
		}
		if (fireSpirits[i]->SlashCollision(slash)) {
			Novice::PlayAudio(damagedSFX, 0, 1.0f);
			fireSpirits[i]->GiveDamage(4);
		}
	}
}


void EnemyManager::DrawEnemy() {
	for (int i = 0; i < kMaxSlime; i++) {
		if (slimes[i]->isSpawn_ == true) {
			slimes[i]->DrawEnemy();
		}
	}

	for (int i = 0; i < kMaxEagle; i++) {
		if (eagles[i]->isSpawn_ == true) {
			eagles[i]->DrawEnemy();
		}
	}

	for (int i = 0; i < kMaxWorm; i++) {
		if (worms[i]->isSpawn_ == true) {
			worms[i]->DrawEnemy();
		}
	}

	for (int i = 0; i < kMaxFireSpirit; i++) {
		if (fireSpirits[i]->isSpawn_ == true) {
			fireSpirits[i]->DrawEnemy();
		}
	}

	for (int i = 0; i < kMaxSpike; i++) {
		if (spikes[i]->isSpawn_ == true) {
			spikes[i]->DrawSpike();
		}
	}
	//Novice::ScreenPrintf(800,0, "slime %d eagle %d worm %d firespirit %d", kMaxSlime, kMaxEagle, kMaxWorm, kMaxFireSpirit);
}

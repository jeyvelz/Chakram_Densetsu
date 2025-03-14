#include <Novice.h>
#include "Player.h"
#include "UI.h"
#include "MapManager.h"
#include "EnemyManager.h"
#include "Boss.h"

#include "Title.h"
#include "StageSelect.h"
#include "StageClear.h"
#include "BGM.h"

const char kWindowTitle[] = "GC1B_07_チャクラム伝説";

enum SCENE_STATE {
	TITLE_SCENE,
	STAGE_SELECT_SCENE,
	INGAME_SCENE,
	STAGE_CLEAR_SCENE,
	GAME_OVER,
	EXIT = 10,
};

enum GAME_STAGE {
	TUTORIAL,
	STAGE1,
	STAGE2,
	BOSS_STAGE,
	STAGE_CLEAR,
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int backgroundHandle = Novice::LoadTexture("./Resources/background.png"); 
	int gameOverHandle = Novice::LoadTexture("./Resources/gameOver.png"); 
	int upHandle = Novice::LoadTexture("./Resources/UI/upButton.png");

	// tutorial images
	//int jumpTutorial = Novice::LoadTexture("./NoviceResources/white1x1.png");

	int selectSFX = Novice::LoadAudio("./Resources/audio/select/select2.mp3");
	int getKeySFX = Novice::LoadAudio("./Resources/audio/sfx/getKey.mp3");

	int scene = TITLE_SCENE;
	int stage = TUTORIAL;

	int isLoad = false;
	int isTitleBgm = false;
	bool isOnExit = false;

	Title* title = new Title;
	int titleReturn = 0;

	StageSelect* stageSelect = new StageSelect;
	StageClear* stageClear = new StageClear;
	int stageClearReturn = 0;

	BGM* bgm = new BGM();

	Player* player = new Player;
	PlayerUI* playerUI = new PlayerUI;
	
	MapManager* map = new MapManager;
	EnemyManager* enemyManager = new EnemyManager();
	Boss* boss = new Boss();
	BossUI* bossUI = new BossUI;
	
	int bossDeadTimer = 0;

	int stageMap[kMapHeight][kMapWidth] = {};

	// game over init
	int deadFrame = 0;

	// shake
	int shakeMax = 1;
	int shakeX = 0;
	int shakeY = 0;

	//int bossDeadTimer = 0;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		/*if (keys[DIK_1]) {
			scene = TITLE_SCENE;
		}
		if (keys[DIK_2]) {
			scene = STAGE_SELECT_SCENE;
		}
		if (keys[DIK_3]) {
			scene = INGAME_SCENE;
		}

		if (keys[DIK_7]) {
			stage = TUTORIAL;
			
		}
		if (keys[DIK_8]) {
			stage = STAGE1;
			
		}
		if (keys[DIK_9]) {
			stage = BOSS_STAGE;
			
		}*/
		
		switch (scene) {
		case (TITLE_SCENE):
			isLoad = false;

			if (isTitleBgm == false) {
				isTitleBgm = true;
				bgm->Stop();
				bgm->PlayBGM(0);
			}
			bgm->Set(0.3f);

			if (titleReturn == 0) {
				titleReturn = title->Select(keys, preKeys);
			}

			if (titleReturn == 1) {
				scene = STAGE_SELECT_SCENE;
		
			} else if (titleReturn == 2) {
				scene = EXIT;
			}

			player->DespawnPlayer();

			title->Draw();
			//Novice::DrawSprite(0, 0, titleBG, 1.0f, 1.0f, 0.0f, WHITE);
			//Novice::ScreenPrintf(0, 0, "TITLE");
		break;
		case(STAGE_SELECT_SCENE):
			isLoad = false;
			titleReturn = 0;
			if (isTitleBgm == false) {
				isTitleBgm = true;
				bgm->Stop();
				bgm->PlayBGM(0);
			}
			bgm->Set(0.3f);

			stageClearReturn = 0;
			enemyManager->DespawnEnemies();
			player->DespawnPlayer();
			player->Move(keys, preKeys, stageMap);
			
			stageSelect->Select(keys, preKeys);

			if (stageSelect->GetStage() == 0) {
				stage = TUTORIAL;
			} else if (stageSelect->GetStage() == 1) {
				stage = STAGE1;
			} else if (stageSelect->GetStage() == 2) {
				stage = STAGE2;
			} else if (stageSelect->GetStage() == 3) {
				stage = BOSS_STAGE;
			}
			
			if (keys[DIK_Z] && !preKeys[DIK_Z]) {
				Novice::PlayAudio(selectSFX, 0, 1.0f);
				scene = INGAME_SCENE;
			}

			stageSelect->Draw();
			
			//Novice::ScreenPrintf(0, 100, "STAGE SELECT");
		break;
		case(INGAME_SCENE):
			if (keys[DIK_F12] && !preKeys[DIK_F12]) {
				scene = TITLE_SCENE;
			}
			isTitleBgm = false;
			switch (stage) {
			case(TUTORIAL):
				
				enemyManager->SetMaxEnemies(2, 1, 1, 0, 3);
				
				if (isLoad == false) {
					isLoad = true;
					for (int i = 0; i < kMapHeight; i++) {
						for (int j = 0; j < kMapWidth; j++) {
							stageMap[i][j] = tutorialMap[i][j];
						}
					}
					player->SpawnPlayer({ 172.0f, 200.0f }); // 172 200

					enemyManager->SpawnSlime({ 1360.0f, 180.0f });
					
					enemyManager->SpawnSlime({ 3000.0f, 250.0f });
					
					enemyManager->SpawnEagle({ 2140.0f, 600.0f }, 120.0f);
					enemyManager->SpawnWorm({ 355.0f, 888.0f }); // pos worm = player center pos - 6;

					enemyManager->SpawnSpike({ 36.0f, 12.0f });
					enemyManager->SpawnSpike({ 37.0f, 12.0f });
					enemyManager->SpawnSpike({ 38.0f, 12.0f });
					bgm->Stop();
					bgm->PlayBGM(1);
				}

				bgm->Set(0.3f);

				player->Move(keys, preKeys, stageMap);
				
				enemyManager->UpdateEnemy(player, stageMap, player->GetPos());
				enemyManager->CheckCollision(player);
				enemyManager->WeaponCollision(player->chakram_, player->slash_);

				map->SetExitPos(137.0f, 962.0f, player->GetScrollPos());

				if (player->BoxCollision(map->GetExitPos(), map->GetExitWidthHalf(), map->GetExitHeightHalf())) {
					isOnExit = true;
					

					if (keys[DIK_UP] && !preKeys[DIK_UP]) {
						scene = STAGE_CLEAR_SCENE;
					}
				} else {
					isOnExit = false;
				}

				
				// Draw Update
				Novice::DrawSprite(static_cast<int>(0 - player->GetScrollPosX()), static_cast<int>(0 + player->GetScrollPosY()),
					backgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);

				map->DrawTutorial(player->GetScrollPos());
				map->DrawExit();
				map->DrawTileSet(0, 0, player->GetScrollPos(), stageMap);
				
				
				enemyManager->DrawEnemy();
				player->Draw();

				if (isOnExit == true) {
					DrawObject({ player->GetPosX(), player->GetPosY() - player->GetScrollPosY() + 100.0f}, 24.0f, 24.0f, 0, 0, upHandle, RIGHT, 0, 0);
				}

				/*Novice::ScreenPrintf(0, 0, "TUTORIAL STAGE");
				Novice::ScreenPrintf(200, 0, "triggerCHeck %d", isOnExit);*/
			break;
			case(STAGE1):
				// load map
				for (int i = 0; i < kMapHeight; i++) {
					for (int j = 0; j < kMapWidth; j++) {
						stageMap[i][j] = stage1Map[i][j];
					}
				}
				enemyManager->SetMaxEnemies(6, 2, 2, 0, 8);
				if (isLoad == false) {
					isLoad = true;
					player->SpawnPlayer({ 170.0f, 1220.0f }); // 170 1220
					
					enemyManager->SpawnSlime({ 1530.0f, 1022.0f });
					enemyManager->SpawnSlime({ 2723.0f, 255.0f });
					enemyManager->SpawnSlime({ 4200.0f, 1010.0f });
					enemyManager->SpawnSlime({ 4300.0f, 200.0f });
					enemyManager->SpawnSlime({ 3700.0f, 200.0f });
					enemyManager->SpawnSlime({ 982.0f, 322.0f });
					
					enemyManager->SpawnEagle({ 4750.0f, 920.0f }, 160.0f);
					// enemyManager->SpawnEagle({ 4208.0f, 422.0f }, 200.0f);
					enemyManager->SpawnEagle({ 1940.0f, 582.0f }, 120.0f);
					
					enemyManager->SpawnWorm({ 2950.0f, 1080.0f });
					enemyManager->SpawnWorm({ 500.0f, 568.0f });

					enemyManager->SpawnSpike({ 28.0f, 14.0f }); /// 1824 912
					enemyManager->SpawnSpike({ 29.0f, 14.0f });

					enemyManager->SpawnSpike({ 29.0f, 2.0f });
					enemyManager->SpawnSpike({ 30.0f, 2.0f });
					enemyManager->SpawnSpike({ 31.0f, 2.0f });
					enemyManager->SpawnSpike({ 32.0f, 2.0f });

					enemyManager->SpawnSpike({ 55.0f, 2.0f });
					enemyManager->SpawnSpike({ 56.0f, 2.0f });
					bgm->Stop();
					bgm->PlayBGM(1);
				}

				bgm->Set(0.3f);
					
				player->Move(keys, preKeys, stageMap);

				enemyManager->UpdateEnemy(player, stageMap, player->chakram_->GetPos());
				enemyManager->CheckCollision(player);				
				enemyManager->WeaponCollision(player->chakram_, player->slash_);
					
				map->SetExitPos(510.0f, 192.0f, player->GetScrollPos());

				if (player->BoxCollision(map->GetExitPos(), map->GetExitWidthHalf(), map->GetExitHeightHalf())) {
					isOnExit = true;
					if (keys[DIK_UP] && !preKeys[DIK_UP]) {
						scene = STAGE_CLEAR_SCENE;
					}
				} else {
					isOnExit = false;
				}

				// Draw Update
				Novice::DrawSprite(static_cast<int>(0 - player->GetScrollPosX()), static_cast<int>(0 + player->GetScrollPosY()),
					backgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
				map->DrawExit();
				map->DrawTileSet(0, 0, player->GetScrollPos(), stageMap);
				
				enemyManager->DrawEnemy();
				player->Draw();

				if (isOnExit == true) {
					DrawObject({ player->GetPosX(), player->GetPosY() - player->GetScrollPosY() + 100.0f }, 24.0f, 24.0f, 0, 0, upHandle, RIGHT, 0, 0);
				}

				//Novice::ScreenPrintf(0, 0, "STAGE 1");

			break;
			case(STAGE2):
				for (int i = 0; i < kMapHeight; i++) {
					for (int j = 0; j < kMapWidth; j++) {
						stageMap[i][j] = stage2Map[i][j];
					}
				}

				enemyManager->SetMaxEnemies(3, 1, 2, 7,15);
				if (isLoad == false) {
					isLoad = true;
					player->SpawnPlayer({ 170.0f, 250.0f }); // 170, 250 
					map->isGetKey_ = false;
					//enemyManager->SpawnEagle({ 1084.0f, 1090.0f }, 120.0f);

					enemyManager->SpawnSlime({ 2800.0f, 710.0f });
					enemyManager->SpawnSlime({ 3700.0f, 960.0f });

					enemyManager->SpawnSlime({ 2915.0f, 320.0f });

					enemyManager->SpawnEagle({ 988.0f, 600.0f }, 120.0f);

					enemyManager->SpawnWorm({ 3752.0f, 504.0f });
					enemyManager->SpawnWorm({ 3852.0f, 504.0f });

					enemyManager->SpawnFireSpirit({ 2000.0f, 1200.0f }, 520.0f);
					enemyManager->SpawnFireSpirit({ 2122.0f, 1122.0f }, 580.0f);
					enemyManager->SpawnFireSpirit({ 1980.0f, 1080.0f }, 520.0f);

					enemyManager->SpawnFireSpirit({ 4880.0f, 1120.0f }, 620.0f);
					enemyManager->SpawnFireSpirit({ 4860.0f, 1100.0f }, 600.0f);

					enemyManager->SpawnFireSpirit({ 1328.0f, 300.0f }, 560.0f);
					enemyManager->SpawnFireSpirit({ 1302.0f, 240.0f }, 580.0f);

					enemyManager->SpawnSpike({ 12.0f, 13.0f });
					enemyManager->SpawnSpike({ 13.0f, 13.0f });
					enemyManager->SpawnSpike({ 14.0f, 13.0f });
					enemyManager->SpawnSpike({ 15.0f, 13.0f });
					enemyManager->SpawnSpike({ 16.0f, 13.0f });
					enemyManager->SpawnSpike({ 17.0f, 13.0f });
					enemyManager->SpawnSpike({ 18.0f, 13.0f });

					enemyManager->SpawnSpike({ 53.0f, 12.0f });
					enemyManager->SpawnSpike({ 54.0f, 12.0f });

					enemyManager->SpawnSpike({ 61.0f, 12.0f });
					enemyManager->SpawnSpike({ 62.0f, 12.0f });

					enemyManager->SpawnSpike({ 29.0f, 2.0f });
					enemyManager->SpawnSpike({ 30.0f, 2.0f });
					enemyManager->SpawnSpike({ 31.0f, 2.0f });
					enemyManager->SpawnSpike({ 32.0f, 2.0f });

					bgm->Stop();
					bgm->PlayBGM(2);
				}
				bgm->Set(0.3f);

				if (map->isGetKey_ == true) {
					
					for (int i = 0; i < kMapHeight; i++) {
						for (int j = 0; j < kMapWidth; j++) {
							if (stageMap[i][j] >= 30) {
								stageMap[i][j] = 0;
							}
						}
					}
				}

				player->Move(keys, preKeys, stageMap);

				enemyManager->UpdateEnemy(player, stageMap, player->chakram_->GetPos());
				enemyManager->CheckCollision(player);
				enemyManager->WeaponCollision(player->chakram_, player->slash_);

				map->SetKeyPos(886.0f, 582.0f, player->GetScrollPos());

				if (player->BoxCollision(map->GetKeyPos(), map->GetKeyWidthHalf(), map->GetKeyHeightHalf())) {
					if (map->isGetKey_ == false) {
						Novice::PlayAudio(getKeySFX, 0, 1.0f);
					}
					map->isGetKey_ = true;
				}

				/// Exit , Map Clear
				map->SetExitPos(4867.0f, 192.0f, player->GetScrollPos());

				if (player->BoxCollision(map->GetExitPos(), map->GetExitWidthHalf(), map->GetExitHeightHalf())) {
					isOnExit = true;
					if (keys[DIK_UP] && !preKeys[DIK_UP]) {
						scene = STAGE_CLEAR_SCENE;
					}
				} else {
					isOnExit = false;
				}


				// Draw Update
				Novice::DrawSprite(static_cast<int>(0 - player->GetScrollPosX()), static_cast<int>(0 + player->GetScrollPosY()),
					backgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
				map->DrawExit();
				map->DrawTileSet(0, 0, player->GetScrollPos(), stageMap);
				map->DrawKey();


				enemyManager->DrawEnemy();
				player->Draw();

				if (isOnExit == true) {
					DrawObject({ player->GetPosX() - player->GetScrollPosX(), player->GetPosY() - player->GetScrollPosY() + 100.0f }, 24.0f, 24.0f, 0, 0, upHandle, RIGHT, 0, 0);
				}

				//Novice::ScreenPrintf(0, 0, "isOnExit %d", isOnExit);

				break;

				//////////////// BOSS STAGE //////////////////
			case(BOSS_STAGE):
				for (int i = 0; i < kMapHeight; i++) {
					for (int j = 0; j < kMapWidth; j++) {
						stageMap[i][j] = bossMap[i][j];
					}
				}
				if (isLoad == false) {
					isLoad = true;
					player->SpawnPlayer({ 600.0f, 220.0f });
					boss->SpawnBoss();
					bgm->Stop();
					bgm->PlayBGM(3);
					bossDeadTimer = 0;
				}
				bgm->Set(0.2f);

				boss->Update(player);
				player->Move(keys, preKeys, stageMap);

				// boss collision
				if (boss->isStomp_ == true) {
					if(player->BoxCollision(boss->GetStompPos(), boss->GetStompWidthHalf(), boss->GetStompHeightHalf())){
						player->Damaged(1);
					}
				}

				/// left foot collision
				if (boss->FootChakramCollision(player->chakram_) ){
					player->chakram_->SpeedUpSpinTimer(30);
				}

				boss->FootSlashCollision(player->slash_);

				/// gem collision
				if (boss->ChakramCollision(player->chakram_)) {
					player->chakram_->SpeedUpSpinTimer(30);
				}

				if (boss->SlashCollision(player->slash_)) {

				}

				// rock collision
				for (int i = 0; i < kMaxRocks; i++) {
					if (boss->rocks->Collision(player->GetPos(), player->GetWidthHalf(), player->GetHeightHalf())) {
						player->Damaged(1);
					}
				}

				for (int i = 0; i < kMaxFireball; i++) {
					if (boss->GetHP() > 0) {
						if (boss->fireballs->Collision(player->GetPos(), player->GetWidthHalf(), player->GetHeightHalf())) {
							player->Damaged(1);
						}
					}
				}

				shakeMax--;
				if (shakeMax < 1) {
					shakeMax = 1;
				}

				shakeX = rand() % shakeMax - shakeMax / 2;
				shakeY = rand() % shakeMax - shakeMax / 2;

				if (boss->GetHP() <= 0) {
					shakeMax = 40;
					bossDeadTimer++;
					if (bossDeadTimer >= 240) {
						scene = STAGE_CLEAR_SCENE;
					}
				}


				/// Draw Update
				Novice::DrawSprite(static_cast<int>(0 - player->GetScrollPosX()), static_cast<int>(0 + player->GetScrollPosY()),
					backgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
				boss->Draw(shakeX, shakeY);
				map->DrawTileSet(0, 0, player->GetScrollPos(), stageMap);
				player->Draw();
				
				bossUI->DrawBossUI(boss->GetHP(), 100, boss->GetStunned(),
					boss->GetLeftFootHP(), boss->GetRightFootHP());

				//Novice::ScreenPrintf(0, 0, "BOSS STAGE %d", bossDeadTimer);
				break;
			default:
				break;
			}
			if (player->GetHP() <= 0) {
				deadFrame++;
				
				if (deadFrame == 30) {
					scene = GAME_OVER;
					deadFrame = 0;
				}
			}
			playerUI->DrawPlayerUI(player->GetHP());
			break;
		///////////// STAGE CLEAR ///////////////////
		case(STAGE_CLEAR_SCENE):
			isTitleBgm = false;
			enemyManager->DespawnEnemies();
			player->DespawnPlayer();

			if (stageClearReturn == 0) {
				stageClearReturn = stageClear->Select(keys, preKeys);
			}

			if (stageClearReturn == 1) {
				scene = TITLE_SCENE;
				
			} else if (stageClearReturn == 2) {
				scene = STAGE_SELECT_SCENE;
			} else if (stageClearReturn == 3) {
				scene = EXIT;
			}

			stageClear->Draw();

			//Novice::ScreenPrintf(0, 0, "STAGE CLEAR");

		break;
		case(GAME_OVER):
			if ((keys[DIK_SPACE] && !preKeys[DIK_SPACE])) {
				scene = TITLE_SCENE;
			}
			Novice::DrawSprite(0, 0, gameOverHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//Novice::ScreenPrintf(0, 0, "GAMEOVER");
			break;
		}

		//Novice::ScreenPrintf(1000, 700, "isLoad %d", isLoad);
		//Novice::ScreenPrintf(800, 700, "title %d clear %d", titleReturn, stageClearReturn);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0 || scene == EXIT) {
			break;
		}
	}
	delete player;
	delete playerUI;
	delete map;
	delete enemyManager;
	delete boss;
	delete bossUI;

	delete title;
	delete stageSelect;
	delete stageClear;
	delete bgm;
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

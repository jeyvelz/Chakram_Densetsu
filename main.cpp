#include <Novice.h>
#include "Player.h"
#include "MapManager.h"

const char kWindowTitle[] = "GC1B_07_ジュナイディ_ジョシュア_タイトル";

enum SCENE_STATE {
	TITLE_SCENE,
	STAGE_SELECT_SCENE,
	INGAME_SCENE,
};

enum GAME_STAGE {
	TUTORIAL,
	STAGE1,
	STAGE2,
	BOSS_STAGE
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int scene = INGAME_SCENE;
	int stage = STAGE1;

	Player* player = new Player;
	MapManager* map = new MapManager;


	int stageMap[kMapHeight][kMapWidth] = {};

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
		switch (scene) {
		case (TITLE_SCENE):

		break;
		case(STAGE_SELECT_SCENE):

		break;
		case(INGAME_SCENE):
			switch (stage) {
			case(TUTORIAL):

			break;
			case(STAGE1):
					for (int i = 0; i < kMapHeight; i++) {
						for (int j = 0; j < kMapWidth; j++) {
							stageMap[i][j] = stage1Map[i][j];
						}
					}
					player->Move(keys, preKeys, stageMap);

					map->DrawTileSet(0, 0, player->GetScrollPos(), stageMap);
					player->Draw();

					Novice::ScreenPrintf(0, 0, "STAGE 1");

			break;
			}
		break;
		}
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
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}
	delete player;
	delete map;
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

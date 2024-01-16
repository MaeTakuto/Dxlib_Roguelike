
#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"
#include "manager/gm_manager.h"
#include "manager/resource_manager.h"
#include "scene/scene_play.h"


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetBackgroundColor(32, 32, 32);

	GameManager::GetInstance(std::make_shared<ScenePlay>());
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	GameManager::GetInstance()->update(delta_time);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

	ResourceManager::destroy();
	GameManager::Destroy();
}

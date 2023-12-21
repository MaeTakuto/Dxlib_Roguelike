#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "scene_title.h"
#include "scene_play.h"


ScenePlay::ScenePlay() {

	is_transition_process_ = false;
}

ScenePlay::~ScenePlay() {

}

void ScenePlay::update(float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene( std::make_shared<SceneTitle>() );
	}
}

void ScenePlay::draw() {

	DrawStringEx(100, 100, -1, "シーンプレイ");
}
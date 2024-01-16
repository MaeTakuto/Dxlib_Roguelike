#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "scene_play.h"
#include "scene_title.h"


SceneTitle::SceneTitle() {

	is_transition_process_ = false;
}

SceneTitle::~SceneTitle() {

}

void SceneTitle::update(float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene(  std::make_shared<ScenePlay>() );
	}
}

void SceneTitle::draw() {

	SetFontSize(80);
	DrawStringEx(100, 100, -1, "タイトルシーン");
}

#pragma once

/*


//-----------------------------------------------------------------------------------------------------------
//
//
// AABBボックス同士の衝突判定と補正サンプル
//
//
//-----------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> boxA = nullptr;
Shared<dxe::Mesh> boxB = nullptr;

float jump_vel = 0;

const float BOX_SIZE = 100;
tnl::Vector3 box_size_A = { BOX_SIZE, BOX_SIZE, BOX_SIZE };
tnl::Vector3 box_size_B = { BOX_SIZE, BOX_SIZE, BOX_SIZE };


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	boxA = dxe::Mesh::CreateCubeMV(BOX_SIZE);
	boxA->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	boxA->pos_ = { 100, 0, 0 };

	boxB = boxA->createClone();
	boxB->pos_ = { -100, 0, 0 };

}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------

	//
	// XZ平面上の移動処理
	//
	tnl::Vector3 before_posA = boxA->pos_;
	tnl::Vector3 before_posB = boxB->pos_;

	tnl::Input::RunIndexKeyDown(
		[&](uint32_t index) {
			tnl::Vector3 v[4] = {
				tnl::Vector3::left,
				tnl::Vector3::right,
				tnl::Vector3::forward,
				tnl::Vector3::back
			};
			boxA->pos_ += v[index] * 3.0f;
		}, eKeys::KB_LEFT, eKeys::KB_RIGHT, eKeys::KB_UP, eKeys::KB_DOWN);


	//
	// ジャンプと落下処理
	//
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) {
		jump_vel = 15.0f;
	}
	boxA->pos_.y += jump_vel;
	if (boxA->pos_.y <= 0) {
		boxA->pos_.y = 0;
		jump_vel = 0;
	}
	else {
		jump_vel -= 0.98f;
	}

	//
	// 衝突判定と補正処理
	//
	if (tnl::IsIntersectAABB(boxA->pos_, box_size_A, boxB->pos_, box_size_B)) {

		//----------------------------------------------------------------------------------------------
		// AABB 同士の座標補正
		// arg1.2.		A, B の移動前の座標
		// arg3.4.		A, B のサイズ
		// arg5.6.		A, B の移動後の座標 ( この関数によって補正後の座標に更新 )
		// arg7.8.9.	横, 縦, 奥 方向の補正タイプ
		// .......		[ PWRFL_A : A が 強い ] [ PWRFL_B : B が 強い ] [ BOTH : A と B は押し合う ]
		// arg10..		補正後に A と B の間に設ける空間 ( デフォルト 1.0f )
		// ret....		矩形 A が B に対してどの方向へ補正されたか
		tnl::eCorrResAABB n = tnl::CorrectPositionAABB(
			before_posA
			, before_posB
			, box_size_A
			, box_size_B
			, boxA->pos_
			, boxB->pos_
			, tnl::eCorrTypeAABB::PWRFL_A
			, tnl::eCorrTypeAABB::PWRFL_B
			, tnl::eCorrTypeAABB::PWRFL_B, 0.1f);

		// 上に補正されたらジャンプ力リセット
		if (tnl::eCorrResAABB::UP == n) jump_vel = 0;
	}


	camera->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	DrawGridGround(camera, 50, 20);

	boxA->render(camera);
	boxB->render(camera);

	DrawDefaultLightGuiController();

	DrawStringEx(0, 0, -1, "カーソル ←↑↓→ 移動 Z でジャンプ");

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}
//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/


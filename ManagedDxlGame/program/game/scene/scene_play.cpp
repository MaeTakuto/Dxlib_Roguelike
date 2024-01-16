#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_title.h"
#include "scene_play.h"
#include "../dungeon/dng_rect.h"
#include "../common/camera.h"
#include "../character/player.h"


ScenePlay::ScenePlay() {

	dng_generator_ = std::make_shared<DungeonManager>();
	camera_ = std::make_shared<Camera>();

	terrain_data_.resize(DungeonManager::FIELD_HEIGHT);
	map_data_.resize(DungeonManager::FIELD_HEIGHT);
	for (int i = 0; i < DungeonManager::FIELD_HEIGHT; i++) {
		terrain_data_[i].resize(DungeonManager::FIELD_WIDTH);
		map_data_[i].resize(DungeonManager::FIELD_WIDTH);
	}

	player_ = std::make_shared<Player>();

	// 画像のロード
	mapchip_gpc_hdl_ =
		ResourceManager::getInstance()->loadAnimation(
			"graphics/mapchip.bmp",
			mapchip_all_size_,
			2,
			1,
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);

	is_transition_process_ = false;
}

ScenePlay::~ScenePlay() {

}

void ScenePlay::update(float delta_time) {

	main_seq_.update(delta_time);

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene( std::make_shared<SceneTitle>() );
	}
}

void ScenePlay::draw() {


	/*for (int y = 0; y < terrain_data_.size(); y++) {
		for (int x = 0; x < terrain_data_[y].size(); x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera_->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", terrain_data_[y][x]);
		}
	}*/

	// マップチップ描画
	for (int y = 0; y < terrain_data_.size(); ++y) {
		for (int x = 0; x < terrain_data_[y].size(); ++x) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x) * GameManager::DRAW_CHIP_SIZE, static_cast<float>(y) * GameManager::DRAW_CHIP_SIZE, 0 }
			- camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

			// 描画
			DrawExtendGraph(draw_pos.x, draw_pos.y,
				draw_pos.x + GameManager::DRAW_CHIP_SIZE,
				draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				mapchip_gpc_hdl_[terrain_data_[y][x]], true);
		}
	}

	player_->draw(camera_);

	// ======= デバッグ ========
	 // DrawStringEx(100, 100, -1, "シーンプレイ");
	 dng_generator_->displayAreaNumber(camera_);
}

// 
void ScenePlay::charaUpdate(float delta_time) {

	player_->update(delta_time);
}

// ==================================================================================
//								メインシーケンス
// ==================================================================================

// ダンジョン生成
bool ScenePlay::seqGenerateDungeon(const float delta_time) {
	
	dng_generator_->generateDungeon();
	terrain_data_ = dng_generator_->getTerrainData();
	map_data_ = dng_generator_->getMapData();

	for (int y = 0; y < map_data_.size(); y++) {
		for (int x = 0; x < map_data_[y].size(); x++) {
			if ( map_data_[y][x] == static_cast<int>( eMapData::PLAYER ) ) {
				tnl::DebugTrace("player x = %d, y = %d\n", x, y);
				tnl::Vector3 pos = { static_cast<float>(x) , static_cast<float>(y), 0 };
				player_->setPos(pos);
			}
		}
	}

	debugMapData();

	camera_->setPos(player_->getPos());

	main_seq_.change(&ScenePlay::seqMain);

	return true;
}

// ダンジョン探索シーケンス
bool ScenePlay::seqMain(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		main_seq_.change(&ScenePlay::seqGenerateDungeon);
	}

	in_dungeon_seq_.update(delta_time);

	// camera_->control();
	camera_->setPos(player_->getPos());


	return true;
}	

// ==================================================================================
//								ダンジョン探索シーケンス
// ==================================================================================

// 
bool ScenePlay::seqPlayerAct(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() == eActState::ACT) {
		
		if ( getMapData( player_->getNextPos() ) == static_cast<int>( eMapData::WALL ) ) {
			tnl::DebugTrace("player\n");
			player_->collisionProcess();
		}
		else {
			in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		}
	}

	return true;
}

// 
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;

	in_dungeon_seq_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// 
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);

	player_->beginAct();
	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);

	return true;
}

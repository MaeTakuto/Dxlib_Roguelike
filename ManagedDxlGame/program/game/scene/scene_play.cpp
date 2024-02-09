#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_title.h"
#include "scene_play.h"
#include "../dungeon/dungeon_manager.h"
#include "../common/camera.h"
#include "../character/player.h"
#include "../manager/enemy_manager.h"


ScenePlay::ScenePlay() {

	dungeon_mgr_ = std::make_shared<DungeonManager>();
	camera_ = std::make_shared<Camera>();
	enemy_mgr_ = std::make_shared<EnemyManager>();
	// enemy_mgr_->setScenePlay(dynamic_pointer_cast<ScenePlay>( GameManager::GetInstance()->getSceneInstance()));

	field_.resize(GameManager::FIELD_HEIGHT);
	map_data_.resize(GameManager::FIELD_HEIGHT);

	areas_.resize(dungeon_mgr_->AREA_MAX);

	for (int i = 0; i < GameManager::FIELD_HEIGHT; i++) {
		field_[i].resize(GameManager::FIELD_WIDTH);
		map_data_[i].resize(GameManager::FIELD_WIDTH);
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

	tnl::DebugTrace("ScenePlayのデストラクタが実行されました\n");
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
	for (int y = 0; y < field_.size(); ++y) {
		for (int x = 0; x < field_[y].size(); ++x) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x) * GameManager::DRAW_CHIP_SIZE, static_cast<float>(y) * GameManager::DRAW_CHIP_SIZE, 0 }
			- camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

			// 描画
			DrawExtendGraph(draw_pos.x, draw_pos.y,
				draw_pos.x + GameManager::DRAW_CHIP_SIZE,
				draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				mapchip_gpc_hdl_[static_cast<int>( field_[y][x].terrain_data ) ], true);

		}
	}


	// デバッグ（ 部屋の入口を表示 ）
	for (int i = 0; i < areas_.size(); i++) {
		for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
			tnl::Vector3 draw_pos = areas_[i].room.entrance[j].pos * GameManager::DRAW_CHIP_SIZE - camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

			DrawBox(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE, -1, true);
		}
	}

	player_->draw(camera_);
	enemy_mgr_->draw(camera_);

	// ======= デバッグ ========
	 // DrawStringEx(100, 100, -1, "シーンプレイ");
	 //dungeon_mgr_->displayAreaNumber(camera_);
}

// 
void ScenePlay::charaUpdate(float delta_time) {

	player_->update(delta_time);
	enemy_mgr_->update(delta_time);
}

// ==================================================================================
//								メインシーケンス
// ==================================================================================

// 
bool ScenePlay::seqSceneStart(const float delta_time) {

	enemy_mgr_->init();

	main_seq_.change(&ScenePlay::seqGenerateDungeon);

	return true;
}

// ダンジョン生成
bool ScenePlay::seqGenerateDungeon(const float delta_time) {
	
	enemy_mgr_->deathAllEnemys();
	dungeon_mgr_->generateDungeon();
	field_ = dungeon_mgr_->getField();
	map_data_ = dungeon_mgr_->getMapData();
	areas_ = dungeon_mgr_->getAreas();

	for (int y = 0; y < map_data_.size(); y++) {
		for (int x = 0; x < map_data_[y].size(); x++) {
			if (field_[y][x].map_data == eMapData::PLAYER ) {
				tnl::DebugTrace("player x = %d, y = %d\n", x, y);
				tnl::Vector3 pos = { static_cast<float>(x) , static_cast<float>(y), 0 };
				player_->setPos(pos);
			}
			else if (field_[y][x].map_data == eMapData::ENEMY) {
				tnl::DebugTrace("enemy x = %d, y = %d\n", x, y);
				enemy_mgr_->spawnEnemy(tnl::Vector3{ static_cast<float>(x) , static_cast<float>(y), 0});
			}
		}
	}

	// debugMapData();

	debugPlaceData();

	camera_->setPos(player_->getPos());

	main_seq_.change(&ScenePlay::seqMain);

	return true;
}

// ダンジョン探索シーケンス
bool ScenePlay::seqMain(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		main_seq_.change(&ScenePlay::seqGenerateDungeon);
		in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
		player_->beginAct();
	}

	// camera_->control();

	 in_dungeon_seq_.update(delta_time);
	 camera_->setPos(player_->getPos());


	return true;
}	

// ==================================================================================
//								ダンジョン探索シーケンス
// ==================================================================================

// 
bool ScenePlay::seqPlayerAct(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() == eActState::MOVE) {
		
		if ( getMapData( player_->getNextPos() ) ==  eMapData::WALL )  { player_->collisionProcess(); }
		else if (getMapData(player_->getNextPos()) == eMapData::ENEMY) { player_->collisionProcess(); }
		else {
			in_dungeon_seq_.change(&ScenePlay::seqEnemyAct);

			setMapData(player_->getPos(), getTerrainData(player_->getPos()));
			setMapData(player_->getNextPos(), eMapData::PLAYER);
		}
	}

	return true;
}

// 
bool ScenePlay::seqEnemyAct(const float delta_time) {

	enemy_mgr_->beginAction();

	in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
	return true;
}

// 
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	if (enemy_mgr_->isAllEnemyActEnd() == false) return true;

	in_dungeon_seq_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// 
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);
	// debugMapData();

	player_->beginAct();
	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);

	return true;
}

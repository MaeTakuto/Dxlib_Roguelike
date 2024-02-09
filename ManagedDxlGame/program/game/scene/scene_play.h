#pragma once
#include "../base/scene_base.h"
#include "../dungeon/dungeon_manager.h"
#include "../character/player.h"

class DungeonManager;
class Camera;
class Player;
class EnemyManager;

// プレイシーンクラス
class ScenePlay : public SceneBase {
public:
	ScenePlay();
	~ScenePlay();

	inline bool isTransitionProcess() override { return is_transition_process_; }

	void update(float delta_time) override;
	void draw() override;

private:
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqSceneStart);
	tnl::Sequence<ScenePlay> in_dungeon_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqPlayerAct);

	std::shared_ptr<DungeonManager> dungeon_mgr_ = nullptr;
	std::shared_ptr<Camera> camera_ = nullptr;
	std::shared_ptr<Player> player_ = nullptr;
	std::shared_ptr<EnemyManager> enemy_mgr_ = nullptr;

	// エリアデータ
	std::vector<Area> areas_;

	// 地形データ
	std::vector< std::vector<Cell> > field_;
	std::vector< std::vector<int> > map_data_;

	// マップチップの画像格納
	std::vector<int> mapchip_gpc_hdl_;
	int mapchip_all_size_ = 2;

	// ========= デバッグ ============
	void debugMapData() {

		tnl::DebugTrace("============== map_data_ ===============\n");

		for (int y = 0; y < map_data_.size(); y++) {
			for (int x = 0; x < map_data_[y].size(); x++) {
				tnl::DebugTrace("%d", static_cast<int>(field_[y][x].map_data));
			}
			tnl::DebugTrace("\n");
		}
	}

	void debugPlaceData() {

		tnl::DebugTrace("============== field_.place ===============\n");

		for (int y = 0; y < map_data_.size(); y++) {
			for (int x = 0; x < map_data_[y].size(); x++) {
				tnl::DebugTrace("%d", static_cast<int>(field_[y][x].place));
			}
			tnl::DebugTrace("\n");
		}
	}

	// キャラクターアップデート
	void charaUpdate(float delta_time);

	// =========== メインシーケンス ===========
	bool seqSceneStart(const float delta_time);
	// bool seqFadeIn(const float delta_time);
	bool seqGenerateDungeon(const float delta_time);
	// bool seqFadeOut(const float delta_time);
	bool seqMain(const float delta_time);

	// =========== ダンジョン探索中のシーケンス ===========
	bool seqPlayerAct(const float delta_time);
	bool seqEnemyAct(const float delta_time);
	bool seqCharaMove(const float delta_time);
	bool seqActEndProcess(const float delta_time);

	// void charaAnimUpdate(float delta_time);

public:
	// ==============================================
	//				インライン関数
	// ==============================================

	// 指定された座標のマップデータを返す
	inline eMapData getMapData(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return eMapData::NONE;
		if (x < 0 || x >= field_[y].size()) return eMapData::NONE;

		return field_[y][x].map_data;
	}

	// マップデータをセット
	inline void setMapData(const tnl::Vector3& pos, eMapData map_data) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		field_[y][x].map_data = map_data;
	}

	// 地形データを取得する
	inline eMapData getTerrainData(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].terrain_data;
	}

	// 部屋か通路かを返す。
	inline ePlace getPlace(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].place;
	}

	// エリア番号を返す
	inline int getAreaId(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return -1;
		if (x < 0 || x >= field_[y].size()) return -1;

		return field_[y][x].area_id;
	}

	// 指定したエリア番号の部屋にプレイヤーが存在するか
	inline bool isPlayerInRoom(int id) {
		if (getPlace(player_->getPos()) != ePlace::ROOM) return false;
		if (getAreaId(player_->getPos()) != id) return false;

		return true;
	}

	inline std::vector<Entrance>& getRoomEntrance(int area_id) {
		return areas_[area_id].room.entrance;
	}

	// 
	inline const tnl::Vector3& getPlayerPos() { return player_->getPos(); }

};
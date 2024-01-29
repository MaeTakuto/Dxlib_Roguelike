#pragma once
#include "../base/scene_base.h"
#include "../dungeon/dungeon_manager.h"

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
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqGenerateDungeon);
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

	// キャラクターアップデート
	void charaUpdate(float delta_time);

	// =========== メインシーケンス ===========
	// bool seqFadeIn(const float delta_time);
	bool seqGenerateDungeon(const float delta_time);
	// bool seqFadeOut(const float delta_time);
	bool seqMain(const float delta_time);

	// =========== ダンジョン探索中のシーケンス ===========
	bool seqPlayerAct(const float delta_time);
	bool seqCharaMove(const float delta_time);
	bool seqActEndProcess(const float delta_time);

	// void charaAnimUpdate(float delta_time);

	// ==============================================
	//				インライン関数
	// ==============================================

	// 指定された座標のマップデータを返す
	inline eMapData getMapData(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].map_data;
	}

};
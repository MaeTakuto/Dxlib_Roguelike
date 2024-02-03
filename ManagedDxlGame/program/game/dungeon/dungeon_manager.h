#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/enum.h"

class Camera;

// 区画のデータクラス
class Rect {
public:
	// 区画の始点座標、終点座標
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

};

class Entrance {
public:
	tnl::Vector3 pos = { 0, 0, 0 };
	int id = 0;
};

class Room {
public:
	// 区画の始点座標、終点座標
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	// 入口の数
	int entrance_count = 0;

	// 部屋の入口の位置
	std::vector<Entrance> entrance;

};

// エリアデータを管理するクラス
class Area {
public:
	Rect area;
	Room room;

	int connect_area_index = -1;
	bool is_connect = false;

	// 通路が存在するかの判定
	eDir connect_area_dir = eDir::NONE;
};

// 1マスのデータを管理するクラス
class Cell {
public:
	// エリアの番号
	int area_id = 0;

	// 場所（ 通路なのか、部屋なのか ）
	ePlace place = ePlace::WALL;

	// マップのデータ（ 衝突判定用 ）
	eMapData map_data = eMapData::WALL;

	// 地形データ（ 床、壁 ）
	eMapData terrain_data = eMapData::WALL;

};

// ダンジョンの生成、管理するクラス
class DungeonManager {
public:
	DungeonManager();
	~DungeonManager();

	static constexpr int FIELD_WIDTH = 64;
	static constexpr int FIELD_HEIGHT = 32;
	const int AREA_MAX_SIZE = 32;
	const int AREA_MIN_SIZE = 16;
	const int AREA_MAX = 8;					// 最大エリア数
	const int AREA_SPACE = 3;
	const int ROOM_AND_ROAD_SPACE = 2;
	const int MAX_ROOM_ENTRANCE = 3;
	const int ENEMY_MAX_INIT_SPAWN = 3;

	// ======= ダンジョン生成、再生成 ======
	void generateDungeon();

	// =============================
	//        デバッグ用
	// =============================

	// 分割したエリアデータを表示
	void displayAreaNumber(const std::shared_ptr<Camera> camera);
	void debugEntranceData() {

		for (int i = 0; i < areas_.size(); i++) {
			tnl::DebugTrace("========== area = %d ==========\n", i);
			for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
				tnl::DebugTrace("entrance[%d].pos = %c\n", j, areas_[i].room.entrance[j].pos.toString().c_str());
				tnl::DebugTrace("entrance[%d].id = %d\n", j, areas_[i].room.entrance[j].id);
			}
			tnl::DebugTrace("\n");
		}
	}

	// ゲッター
	inline std::vector< std::vector<Cell> >& getField() { return field_; }
	inline std::vector< std::vector<int> >& getMapData() { return map_data_; }
	inline std::vector<Area>& getAreas() { return areas_; }

private:
	// int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// 地形データ
	std::vector<std::vector<Cell> > field_{ FIELD_HEIGHT, std::vector<Cell>( FIELD_WIDTH ) };
	std::vector<std::vector<int> > map_data_{ FIELD_HEIGHT, std::vector<int>( FIELD_WIDTH ) };

	int area_count_ = 0;
	std::vector<Area> areas_;

	// 通路作成用
	int* order_connect_rooms_ = nullptr;				// ルームを繋げる順番
	int order_index_ = 0;								// orderの添え字
	int connect_error_count_ = 0;						// エラーの発見用

	// startからendまでの範囲で乱数生成
	inline int random(int start, int end) { return rand() % (end - start + 1) + start; }

	void areaDataInit();
	void fieldDataInit();
	
	// エリアを分割
	void splitArea(int area_index, int vertical);
	
	// フィールドのエリア番号を設定
	void settingAreaNumber();

	// 部屋を作成
	void createRoom();

	// ======== 未実装 =========
	void createSmallRoom(int area_index);
	void createMidiumRoom(int area_index);
	// =========================

	// フィールド作成
	void generateRoom();

	void getNextConnectRoomIndex(int area_index);

	// 部屋同士を繋げる
	void connectRoom(int area_index);

	// 部屋同士を繋げるエリアの番号をセット
	bool checkConnectAreaNumMax(int x, int y, int area_index);

	// 上下、左右の部屋のに通路を作成
	void connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y);
	void connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y);

	// マップデータの作成（ 地形データ、各キャラの位置データ ）
	void generateMapData();

	// プレイヤーの生成位置を決める
	void spawnPlayer();

	// 敵の生成位置を決める
	void spawnEnemy();

};
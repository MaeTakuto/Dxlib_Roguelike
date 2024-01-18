#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class Camera;

// 通路
enum class eDir {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

// マップデータ
enum class eMapData {
	GROUND = 0,
	WALL,
	PLAYER,
	ENEMY
};

enum class ePlace {
	ROAD = 0,
	ROOM
};

// 区画のデータクラス
class Rect {
public:
	// 区画の始点座標、終点座標
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

};

class Area {
public:
	Rect area;
	Rect room;

	// 通路が存在するかの判定
	eDir connect_area_dir = eDir::NONE;

	int connect_area_index = -1;
	bool is_connect = false;
};

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

	// ダンジョン生成、再生成
	void generateDungeon();

	// =============================
	//        デバッグ用
	// =============================

	// 分割したエリアデータを表示
	void displayAreaNumber(const std::shared_ptr<Camera> camera);

	inline std::vector< std::vector<int> >& getTerrainData() { return terrain_data_; }
	inline std::vector< std::vector<int> >& getMapData() { return map_data_; }

private:
	int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// 地形データ
	std::vector<std::vector<int> > terrain_data_{ FIELD_HEIGHT, std::vector<int>( FIELD_WIDTH ) };
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
	void generateField();

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
#pragma once

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
	WALL = 0,
	ROAD,
	ROOM,
};

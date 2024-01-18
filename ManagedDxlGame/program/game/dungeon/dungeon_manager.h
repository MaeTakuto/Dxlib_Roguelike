#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class Camera;

// �ʘH
enum class eDir {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

// �}�b�v�f�[�^
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

// ���̃f�[�^�N���X
class Rect {
public:
	// ���̎n�_���W�A�I�_���W
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

};

class Area {
public:
	Rect area;
	Rect room;

	// �ʘH�����݂��邩�̔���
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
	const int AREA_MAX = 8;					// �ő�G���A��
	const int AREA_SPACE = 3;
	const int ROOM_AND_ROAD_SPACE = 2;

	// �_���W���������A�Đ���
	void generateDungeon();

	// =============================
	//        �f�o�b�O�p
	// =============================

	// ���������G���A�f�[�^��\��
	void displayAreaNumber(const std::shared_ptr<Camera> camera);

	inline std::vector< std::vector<int> >& getTerrainData() { return terrain_data_; }
	inline std::vector< std::vector<int> >& getMapData() { return map_data_; }

private:
	int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// �n�`�f�[�^
	std::vector<std::vector<int> > terrain_data_{ FIELD_HEIGHT, std::vector<int>( FIELD_WIDTH ) };
	std::vector<std::vector<int> > map_data_{ FIELD_HEIGHT, std::vector<int>( FIELD_WIDTH ) };

	int area_count_ = 0;
	std::vector<Area> areas_;

	// �ʘH�쐬�p
	int* order_connect_rooms_ = nullptr;				// ���[�����q���鏇��
	int order_index_ = 0;								// order�̓Y����
	int connect_error_count_ = 0;						// �G���[�̔����p

	// start����end�܂ł͈̔͂ŗ�������
	inline int random(int start, int end) { return rand() % (end - start + 1) + start; }

	void areaDataInit();
	void fieldDataInit();
	
	// �G���A�𕪊�
	void splitArea(int area_index, int vertical);
	
	// �t�B�[���h�̃G���A�ԍ���ݒ�
	void settingAreaNumber();

	// �������쐬
	void createRoom();

	// ======== ������ =========
	void createSmallRoom(int area_index);
	void createMidiumRoom(int area_index);
	// =========================

	// �t�B�[���h�쐬
	void generateField();

	void getNextConnectRoomIndex(int area_index);

	// �������m���q����
	void connectRoom(int area_index);

	// �������m���q����G���A�̔ԍ����Z�b�g
	bool checkConnectAreaNumMax(int x, int y, int area_index);

	// �㉺�A���E�̕����̂ɒʘH���쐬
	void connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y);
	void connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y);

	// �}�b�v�f�[�^�̍쐬�i �n�`�f�[�^�A�e�L�����̈ʒu�f�[�^ �j
	void generateMapData();

	// �v���C���[�̐����ʒu�����߂�
	void spawnPlayer();

	// �G�̐����ʒu�����߂�
	void spawnEnemy();

};
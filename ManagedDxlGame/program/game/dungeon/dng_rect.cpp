#include "../../dxlib_ext/dxlib_ext.h"
#include "dng_rect.h"
#include "../common/camera.h"


// �R���X�g���N�^
DungeonManager::DungeonManager() {

	areas_.resize(AREA_MAX);

	order_connect_rooms_ = new int[AREA_MAX];

	for (int i = 0; i < AREA_MAX; i++) {
		order_connect_rooms_[i] = 0;
	}

}

// 
DungeonManager::~DungeonManager() {

	delete[] order_connect_rooms_;
}

// �_���W��������
void DungeonManager::generateDungeon() {

	areaDataInit();
	fieldDataInit();

	// �G���A�𕪊�
	splitArea(0, rand() % 2);
	if (areas_[0].area.width >= AREA_MAX_SIZE && areas_[0].area.height >= AREA_MAX_SIZE) { splitArea(0, rand() % 2); }

	settingAreaNumber();

	for (int i = 0; i < area_count_; i++) {
		tnl::DebugTrace("----- index = %d ------\n", i);
		tnl::DebugTrace("x = %d, y = %d, width = %d, height = %d\n", areas_[i].area.x, areas_[i].area.y, areas_[i].area.width, areas_[i].area.height);
	}

	createRoom();
	generateField();

	// �ʘH���쐬
	while (order_index_ + 1 < area_count_) {
		connectRoom(order_connect_rooms_[order_index_]);
		tnl::DebugTrace("order_index + 1 = %d\n", order_index_ + 1);
	}

	generateMapData();
	spawnPlayer();

}

// �G���A�̏�����
void DungeonManager::areaDataInit() {

	for (int i = 0; i < AREA_MAX; i++) {
		areas_[i].area.x = 0;
		areas_[i].area.y = 0;
		areas_[i].area.width  = 0;
		areas_[i].area.height = 0;

		areas_[i].room.x = 0;
		areas_[i].room.y = 0;
		areas_[i].room.width = 0;
		areas_[i].room.height = 0;

		areas_[i].connect_area_index = -1;
		areas_[i].connect_area_dir = eDir::NONE;
		areas_[i].is_connect = false;

		order_connect_rooms_[i] = 0;
	}

	area_count_ = 1;

	areas_[0].area.x = 0;
	areas_[0].area.y = 0;
	areas_[0].area.width = FIELD_WIDTH;
	areas_[0].area.height = FIELD_HEIGHT;
	areas_[0].is_connect = true;;

	order_index_ = 0;
	connect_error_count_ = 0;
}

// �t�B�[���h�f�[�^�̏�����
void DungeonManager::fieldDataInit() {

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			terrain_data_[y][x] = static_cast<int>(eMapData::WALL);
		}
	}
}

// �G���A�������_���ɕ���
void DungeonManager::splitArea(int area_index, int vertical) {

	if (area_count_ >= AREA_MAX) return;

	int new_area_index = area_count_;
	int width = areas_[area_index].area.width;
	int height = areas_[area_index].area.height;

	// ���𕪊�
	if (vertical) {

		if (areas_[area_index].area.height / 2 < AREA_MIN_SIZE) return;

		areas_[area_index].area.height /= 2;

		areas_[new_area_index].area.x = areas_[area_index].area.x;
		areas_[new_area_index].area.y = areas_[area_index].area.y + areas_[area_index].area.height;
		areas_[new_area_index].area.width = areas_[area_index].area.width;
		areas_[new_area_index].area.height = height - areas_[area_index].area.height;

	}
	else {
		if (areas_[area_index].area.width / 2 < AREA_MIN_SIZE) return;

		areas_[area_index].area.width /= 2;

		areas_[new_area_index].area.x = areas_[area_index].area.x +  areas_[area_index].area.width;
		areas_[new_area_index].area.y = areas_[area_index].area.y;
		areas_[new_area_index].area.width = width - areas_[area_index].area.width;
		areas_[new_area_index].area.height = areas_[area_index].area.height;

	}

	area_count_++;
	splitArea(area_index, rand() % 2);
	splitArea(new_area_index, rand() % 2);

	if (areas_[area_index].area.width > AREA_MAX_SIZE) { splitArea(area_index, 0); }
	if (areas_[new_area_index].area.width > AREA_MAX_SIZE) { splitArea(new_area_index, 0); }
}

// �G���A�ԍ����G���A�f�[�^�ɐݒ�
void DungeonManager::settingAreaNumber() {

	tnl::DebugTrace("area_count = %d\n", area_count_);

	// ���������G���A�f�[�^��\��
	for (int i = 0; i < area_count_; i++) {

		for (int y = areas_[i].area.y; y < areas_[i].area.y + areas_[i].area.height; y++) {
			for (int x = areas_[i].area.x; x < areas_[i].area.x + areas_[i].area.width; x++) {
				area_number_[y][x] = i;
			}
		}
	}
}

// �������쐬
void DungeonManager::createRoom() {

	for (int area_index = 0; area_index < area_count_; area_index++) {

		int rand_x = rand() % (AREA_SPACE);
		int rand_y = rand() % (AREA_SPACE);
		int rand_width = rand() % (AREA_SPACE) + rand_x;
		int rand_height = rand() % (AREA_SPACE) + rand_y;

		areas_[area_index].room.x = areas_[area_index].area.x + AREA_SPACE + rand_x;
		areas_[area_index].room.y = areas_[area_index].area.y + AREA_SPACE + rand_y;
		areas_[area_index].room.width = areas_[area_index].area.width - ( AREA_SPACE * 2 ) - rand_width;
		areas_[area_index].room.height = areas_[area_index].area.height - ( AREA_SPACE * 2 ) - rand_height;
	}

}

// ���������쐬
void DungeonManager::createSmallRoom(int area_index) {
	
	int rand_x = rand() % (AREA_SPACE);
	int rand_y = rand() % (AREA_SPACE);
	int rand_width = rand() % (AREA_SPACE) + rand_x;
	int rand_height = rand() % (AREA_SPACE) + rand_y;

	areas_[area_index].room.x = areas_[area_index].area.x + AREA_SPACE + rand_x;
	areas_[area_index].room.y = areas_[area_index].area.y + AREA_SPACE + rand_y;
	areas_[area_index].room.width = areas_[area_index].area.width - ( AREA_SPACE * 2 ) - rand_width;
	areas_[area_index].room.height = areas_[area_index].area.height - ( AREA_SPACE * 2 ) - rand_height;
}

// ������
void DungeonManager::createMidiumRoom(int area_index) {

}


// 
void DungeonManager::connectRoom(int area_index) {

	int neighbor_index;

	tnl::DebugTrace("========== connectRoom(area_index = %d) ===========\n", area_index);

	getNextConnectRoomIndex(area_index);

	neighbor_index = areas_[area_index].connect_area_index;

	// ���̃G���A�����������ꍇ
	if (areas_[area_index].connect_area_index != -1) {

		areas_[neighbor_index].is_connect = true;
		order_connect_rooms_[order_index_ + 1] = neighbor_index;
		order_index_++;

		// ============================ �ʘH���쐬 ===============================

		// d1�F�ړ��O�̃h�A�̍��W�Ad2�F�ړ���̃h�A�̍��W
		int dx1, dy1, dx2, dy2;

		int index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
		tnl::DebugTrace("order[%d] = %d\n", order_index_, order_connect_rooms_[order_index_]);
		tnl::DebugTrace("d1 index = %d\n", index);

		// �����[���̓K���ȍ��W�w��
		dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
		dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);

		index = order_connect_rooms_[order_index_];
		tnl::DebugTrace("d2 index = %d\n", index);
		dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
		dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);

		tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);

		connect_error_count_ = 0;

		// �e��������ʘH��L�΂�
		// 
		switch (areas_[area_index].connect_area_dir) {
		case eDir::UP:
			tnl::DebugTrace("��ʘH�쐬\n");
			connectUpAndDownRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir::DOWN:
			tnl::DebugTrace("���ʘH�쐬\n");
			connectUpAndDownRooms(dx1, dy1, dx2, dy2);
			break;

		case eDir::LEFT:
			tnl::DebugTrace("���ʘH�쐬\n");
			connectLeftAndRightRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir::RIGHT:
			tnl::DebugTrace("���ʘH�쐬\n");
			connectLeftAndRightRooms(dx1, dy1, dx2, dy2);
			break;
		}
	}
	// ���������ꍇ
	else {
		connect_error_count_++;
		tnl::DebugTrace("connect_error_�F%d\n", connect_error_count_);
		tnl::DebugTrace("order_index_�F%d\n", order_index_);

		if (order_index_ < connect_error_count_) {
			order_index_ = area_count_;
			return;
		}

		int prev_index = order_connect_rooms_[order_index_ - connect_error_count_];
		areas_[prev_index].connect_area_index = -1;

		connectRoom(prev_index);
	}
}

// 
void DungeonManager::getNextConnectRoomIndex(int area_index) {

	int x, y;

	// for���[�v�I���̕ϐ�
	int ex, ey;

	if (areas_[area_index].area.y > 0) {

		tnl::DebugTrace("��m�F\n");

		y = areas_[area_index].area.y - 1;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[area_number_[y][x]].area.width) {

			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
			if (areas_[area_number_[y][x]].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::UP;
		}
	}
	// -------- ���� --------
	if (areas_[area_index].area.y + areas_[area_index].area.height < FIELD_HEIGHT) {

		tnl::DebugTrace("���m�F\n");

		y = areas_[area_index].area.y + areas_[area_index].area.height;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[area_number_[y][x]].area.width) {

			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
			if (areas_[area_number_[y][x]].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::DOWN;
		}
	}
	// -------- ���� --------
	if ( areas_[area_index].area.x > 0 ) {

		tnl::DebugTrace("���m�F\n");

		x = areas_[area_index].area.x - 1;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
		for (y = areas_[area_index].area.y; y < ey; y += areas_[area_number_[y][x]].area.height)
		{
			if (areas_[area_number_[y][x]].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::LEFT;
		}
	}
	// -------- �E�� --------
	if (areas_[area_index].area.x + areas_[area_index].area.width < FIELD_WIDTH) {

		tnl::DebugTrace("�E�m�F\n");

		x = areas_[area_index].area.x + areas_[area_index].area.width;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
		for (y = areas_[area_index].area.y; y < ey; y += areas_[area_number_[y][x]].area.height)
		{
			if (areas_[area_number_[y][x]].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::RIGHT;
		}
	}
}

// 
//void DungeonGenerator::connectRoom(int area_index) {
//
//	tnl::DebugTrace("========== connectRoom(area_index = %d) ===========\n", area_index);
//	
//	// areaIndex�Ԗڂ̃G���A�ɗאڂ���G���A�ԍ��̍ő�l��neighbor_area_num[0]�Ɋi�[�ł���
//	// int neighbor_area_num[2] = { -1, 0 };
//	int neighbor_area_num = -1;
//
//	// ���̕���
//	eDir next_direction = eDir::MAX;
//
//	// areaIndex�Ԗڂ̃G���A�ɗאڂ���G���A�̈�ԑ傫���G���A�ԍ���������
//	// -------- �㑤 --------
//	if (areas_[area_index].area.y > 0) {
//
//		tnl::DebugTrace("��m�F\n");
//
//		y = areas_[area_index].area.y - 1;
//		ex = areas_[area_index].area.x + areas_[area_index].area.width;
//
//		for (x = areas_[area_index].area.x; x < ex; x += areas_[area_number_[y][x]].area.width) {
//
//			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
//			if (areas_[area_number_[y][x]].connect_area_index) continue;
//
//			if (tryGetMaxAreaNumber(x, y, neighbor_area_num)) next_direction = eDir::UP;
//		}
//	}
//
//	// -------- ���� --------
//	if (areas_[area_index].area.y + areas_[area_index].area.height < FIELD_HEIGHT) {
//
//		tnl::DebugTrace("���m�F\n");
//
//		y = areas_[area_index].area.y + areas_[area_index].area.height;
//		ex = areas_[area_index].area.x + areas_[area_index].area.width;
//
//		for (x = areas_[area_index].area.x; x < ex; x += areas_[area_number_[y][x]].area.width) {
//
//			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
//			if (areas_[area_number_[y][x]].connect_area_index) continue;
//
//			if (tryGetMaxAreaNumber(x, y, neighbor_area_num)) next_direction = eDir::DOWN;
//		}
//	}
//
//	// -------- ���� --------
//	if ( areas_[area_index].area.x > 0 ) {
//
//		tnl::DebugTrace("���m�F\n");
//
//		x = areas_[area_index].area.x - 1;
//		ey = areas_[area_index].area.y + areas_[area_index].area.height;
//
//		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
//		for (y = areas_[area_index].area.y; y < ey; y += areas_[area_number_[y][x]].area.height)
//		{
//			if (areas_[area_number_[y][x]].connect_area_index) continue;
//
//			if (tryGetMaxAreaNumber(x, y, neighbor_area_num)) next_direction = eDir::LEFT;
//		}
//	}
//
//	// -------- �E�� --------
//	if (areas_[area_index].area.x + areas_[area_index].area.width < FIELD_WIDTH) {
//
//		tnl::DebugTrace("�E�m�F\n");
//
//		x = areas_[area_index].area.x + areas_[area_index].area.width;
//		ey = areas_[area_index].area.y + areas_[area_index].area.height;
//
//		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
//		for (y = areas_[area_index].area.y; y < ey; y += areas_[area_number_[y][x]].area.height)
//		{
//			if (areas_[area_number_[y][x]].connect_area_index) continue;
//
//			if (tryGetMaxAreaNumber(x, y, neighbor_area_num)) next_direction = eDir::RIGHT;
//		}
//	}
//
//	// ���̃G���A�����������ꍇ
//	if (neighbor_area_num != -1) {
//		areas_[neighbor_area_num].connect_area_index = true;
//		order_[order_index_ + 1] = neighbor_area_num;
//		order_index_++;
//
//		// ============================ �ʘH���쐬 ===============================
//
//		// d1�F�ړ��O�̃h�A�̍��W�Ad2�F�ړ���̃h�A�̍��W
//		int dx1, dy1, dx2, dy2;
//
//		int index = order_[order_index_ - 1 - connect_error_count_];
//		tnl::DebugTrace("order[%d] = %d\n", order_index_, order_[order_index_]);
//		tnl::DebugTrace("d1 index = %d\n", index);
//
//		// �����[���̓K���ȍ��W�w��
//		dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
//		dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
//
//		index = order_[order_index_];
//		tnl::DebugTrace("d2 index = %d\n", index);
//		dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
//		dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
//
//		tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
//
//		connect_error_count_ = 0;
//
//		// �e��������ʘH��L�΂�
//		// 
//		switch (next_direction) {
//		case eDir::UP:
//			tnl::DebugTrace("��ʘH�쐬\n");
//			connectUpAndDownRooms(dx2, dy2, dx1, dy1);
//			break;
//
//		case eDir::DOWN:
//			tnl::DebugTrace("���ʘH�쐬\n");
//			connectUpAndDownRooms(dx1, dy1, dx2, dy2);
//			break;
//
//		case eDir::LEFT:
//			tnl::DebugTrace("���ʘH�쐬\n");
//			connectLeftAndRightRooms(dx2, dy2, dx1, dy1);
//			break;
//
//		case eDir::RIGHT:
//			tnl::DebugTrace("���ʘH�쐬\n");
//			connectLeftAndRightRooms(dx1, dy1, dx2, dy2);
//			break;
//		}
//	}
//	// ���������ꍇ
//	else {
//		connect_error_count_++;
//		tnl::DebugTrace("connect_error_�F%d\n", connect_error_count_);
//		tnl::DebugTrace("order_index_�F%d\n", order_index_);
//		connectRoom(order_[order_index_ - connect_error_count_]);
//	}
//
//	if (order_index_ + 1 >= area_count_) return;
//
//	connectRoom(neighbor_area_num);
//
//}

//
bool DungeonManager::checkConnectAreaNumMax(int x, int y, int area_index) {

	if (areas_[area_index].connect_area_index < area_number_[y][x]) {

		areas_[area_index].connect_area_index = area_number_[y][x];
		tnl::DebugTrace("�אڂ���G���A�ԍ��̍ő�l���X�V�B�G���A�ԍ��F%d\n", areas_[area_index].connect_area_index);
		return true;
	}

	return false;
}

// �אڂ��镔���̏㉺�̒ʘH�쐬
void DungeonManager::connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y) {

	tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
	while (down_y != up_y) {
		if (abs(down_y - up_y) == 1) {
			down_y--;
			tnl::DebugTrace("down_y = %d\n", down_y);
			terrain_data_[down_y][down_x] = static_cast<int>(eMapData::GROUND);
		}
		else {
			down_y--, up_y++;
			tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
			terrain_data_[down_y][down_x] = static_cast<int>(eMapData::GROUND);
			terrain_data_[up_y][up_x] = static_cast<int>(eMapData::GROUND);
		}
	}

	if (down_y == up_y) {
		while (down_x != up_x) {
			if (down_x < up_x) down_x++;
			else down_x--;
			terrain_data_[down_y][down_x] = static_cast<int>(eMapData::GROUND);
		}
	}
}

// �אڂ��镔���̍��E�̒ʘH�쐬
void DungeonManager::connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y) {
	
	while (left_x != right_x) {
		if (abs(left_x - right_x) == 1) {
			left_x++;
			terrain_data_[left_y][left_x] = static_cast<int>(eMapData::GROUND);
		}
		else {
			left_x++, right_x--;
			terrain_data_[left_y][left_x] = static_cast<int>(eMapData::GROUND);
			terrain_data_[right_y][right_x] = static_cast<int>(eMapData::GROUND);
		}
	}

	if (left_x == right_x) {
		while (left_y != right_y) {
			if (left_y < right_y) left_y++;
			else left_y--;
			terrain_data_[left_y][left_x] = static_cast<int>(eMapData::GROUND);
		}
	}
}

//void DungeonGenerator::connectRoom() {
//
//	for (int area_index = 0; area_index < area_count_; area_index++) {
//
//		// �G���A�̌q����ʘH�̊m��
//		desideDirectionRoad(area_index);
//
//		// ������ɒʘH���쐬
//		if (area_data_[area_index].is_road[static_cast<int>(eRoad::UP)]) {
//			createUpRoad(area_index);
//		}
//		// ���ʘH
//		if (area_data_[area_index].is_road[static_cast<int>(eRoad::DOWN)]) {
//			createDownRoad(area_index);
//		}
//		// ���ʘH
//		if (area_data_[area_index].is_road[static_cast<int>(eRoad::LEFT)]) {
//			createLeftRoad(area_index);
//		}
//		// �E�ʘH
//		if (area_data_[area_index].is_road[static_cast<int>(eRoad::RIGHT)]) {
//			createRightRoad(area_index);
//		}
//	}
//}

//// 
//void DungeonGenerator::createUpRoad(int area_index) {
//
//	int random_range = 0;		// �ʘH�̍쐬���W�̗����͈̔�
//	bool is_connect = false;
//
//	// ���̃T�C�Y��32�ȏ�̏ꍇ�A�쐬����ʘH��1��2�������߂č쐬�B
//	if (areas_[area_index].area.width >= AREA_MAX_SIZE) {
//		int road_num = rand() % 2;
//
//		// 2�ʘH���쐬����ꍇ
//		if (road_num) {
//
//			random_range = (areas_[area_index].room.width / 2) - (ROOM_AND_ROAD_SPACE);
//			
//			// �����̍��������̒ʘH���쐬
//			int rand_x = rand() % random_range + ROOM_AND_ROAD_SPACE + areas_[area_index].room.x;
//
//			for (int y = areas_[area_index].room.y - 1; y >= areas_[area_index].area.y; y--) {
//				terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			// �����̉E�������̒ʘH���쐬
//			rand_x = rand() % random_range + ROOM_AND_ROAD_SPACE + (areas_[area_index].room.width / 2) + areas_[area_index].room.x;
//
//			for (int y = areas_[area_index].room.y - 1; y >= areas_[area_index].area.y; y--) {
//				terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			is_connect = true;
//		}
//	}
//
//	// �ʘH���܂��쐬�ł��Ă��Ȃ��ꍇ
//	if (!is_connect) {
//
//		random_range = (areas_[area_index].room.width) - (ROOM_AND_ROAD_SPACE);
//		int rand_x = (rand() % random_range) + ROOM_AND_ROAD_SPACE + areas_[area_index].room.x;
//
//		for (int y = areas_[area_index].room.y - 1; y >= areas_[area_index].area.y; y--) {
//			terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//		}
//	}
//}
//
//
//// 
//void DungeonGenerator::createDownRoad(int area_index) {
//	
//	int random_range = 0;		// �ʘH�̍쐬���W�̗����͈̔�
//	bool is_connect = false;
//
//	// for���̏����l
//	int start_y = areas_[area_index].room.y + areas_[area_index].room.height;
//
//	// ���̃T�C�Y��32�ȏ�̏ꍇ�A�쐬����ʘH��1��2�������߂č쐬�B
//	if (areas_[area_index].area.width >= AREA_MAX_SIZE) {
//		int road_num = rand() % 2;
//
//		// 2�ʘH���쐬����ꍇ
//		if (road_num) {
//
//			// �����̍��������̒ʘH���쐬
//			random_range = (areas_[area_index].room.width / 2) - (ROOM_AND_ROAD_SPACE);
//			int rand_x = rand() % random_range + ROOM_AND_ROAD_SPACE + areas_[area_index].room.x;
//
//			for (int y = start_y; y <= areas_[area_index].area.height; y++) {
//				terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			random_range = (areas_[area_index].room.width / 2) - (ROOM_AND_ROAD_SPACE);
//			rand_x = rand() % random_range + ROOM_AND_ROAD_SPACE + (areas_[area_index].room.width / 2) + areas_[area_index].room.x;
//
//			for (int y = start_y; y <= areas_[area_index].area.height; y++) {
//				terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			is_connect = true;
//		}
//	}
//
//	// �ʘH���܂��쐬�ł��Ă��Ȃ��ꍇ
//	if (!is_connect) {
//
//		random_range = (areas_[area_index].room.width) - (ROOM_AND_ROAD_SPACE);
//		int rand_x = (rand() % random_range) + ROOM_AND_ROAD_SPACE + areas_[area_index].room.x;
//
//		for (int y = start_y; y <= areas_[area_index].area.height; y++) {
//			terrain_data_[y][rand_x] = static_cast<int>(eMapData::GROUND);
//		}
//	}
//
//	// �G���A�̉��ɉ��ʘH���쐬
//	int y = areas_[area_index].area.y + areas_[area_index].area.height;
//	int end_x = areas_[area_index].area.x + areas_[area_index].area.width - 1;
//
//	for (int x = areas_[area_index].area.x + 1; x < end_x; x++) {
//		terrain_data_[y][x] = static_cast<int>(eMapData::GROUND);
//	}
//}
//
//// 
//void DungeonGenerator::createLeftRoad(int area_index) {
//
//	int random_range = 0;		// �ʘH�̍쐬���W�̗����͈̔�
//	bool is_connect = false;
//
//	// ���̃T�C�Y��32�ȏ�̏ꍇ�A�쐬����ʘH��1��2�������߂č쐬�B
//	if (areas_[area_index].area.height >= AREA_MAX_SIZE) {
//		int road_num = rand() % 2;
//
//		// 2�ʘH���쐬����ꍇ
//		if (road_num) {
//
//			// �����̍��������̒ʘH���쐬
//			random_range = (areas_[area_index].room.height / 2) - (ROOM_AND_ROAD_SPACE);
//			int rand_y = rand() % random_range + ROOM_AND_ROAD_SPACE + areas_[area_index].room.y;
//
//			for (int x = areas_[area_index].room.x - 1; x >= areas_[area_index].area.x; x--) {
//				terrain_data_[rand_y][x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			rand_y = rand() % random_range + ROOM_AND_ROAD_SPACE + areas_[area_index].room.y + ( areas_[area_index].room.height / 2 );
//
//			for (int x = areas_[area_index].room.x - 1; x >= areas_[area_index].area.x; x--) {
//				terrain_data_[rand_y][x] = static_cast<int>(eMapData::GROUND);
//			}
//
//			is_connect = true;
//		}
//	}
//
//	// �ʘH���܂��쐬�ł��Ă��Ȃ��ꍇ
//	if (!is_connect) {
//
//		random_range = (areas_[area_index].room.height) - (ROOM_AND_ROAD_SPACE);
//		int rand_y = (rand() % random_range) + ROOM_AND_ROAD_SPACE + areas_[area_index].room.y;
//
//		for (int x = areas_[area_index].room.x - 1; x >= areas_[area_index].area.x; x--) {
//			terrain_data_[rand_y][x] = static_cast<int>(eMapData::GROUND);
//		}
//	}
//}
//
//// 
//void DungeonGenerator::createRightRoad(int area_index) {
//
//}
//
//// �����̒ʘH���쐬������������߂�
//void DungeonGenerator::desideDirectionRoad(int area_index) {
//
//	int road_conut = 0;
//
//	for (int i = 0; i < static_cast<int>( eDir::MAX ); i++ ) {
//		if (areas_[area_index].is_road[i]) road_conut++;
//	}
//
//	// �ʘH��2�ȉ��̏ꍇ�A�֐��𔲂���
//	if (road_conut <= 2) return;
//	
//	while (1) {
//		int road_index = rand() % static_cast<int>(eDir::MAX);
//		if (areas_[area_index].is_road[road_index]) {
//			areas_[area_index].is_road[road_index] = false;
//			break;
//		}
//	}
//
//	return;
//}

// �t�B�[���h���쐬
void DungeonManager::generateField() {

	for (int i = 0; i < area_count_; i++) {

		tnl::DebugTrace("area = %d, x = %d, y = %d, width = %d, height = %d\n", i, areas_[i].room.x, areas_[i].room.y, areas_[i].room.width, areas_[i].room.height);
		for (int y = areas_[i].room.y; y < areas_[i].room.y + areas_[i].room.height; y++) {
			for (int x = areas_[i].room.x; x < areas_[i].room.x + areas_[i].room.width; x++) {
				terrain_data_[y][x] = static_cast<int>(eMapData::GROUND);
			}
		}
	}
}

void DungeonManager::displayAreaNumber(const std::shared_ptr<Camera> camera) {

	SetFontSize(DEFAULT_FONT_SIZE);

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", area_number_[y][x]);
		}
	}
}

void DungeonManager::generateMapData() {

	for (int y = 0; y < terrain_data_.size(); y++) {
		for (int x = 0; x < terrain_data_[y].size(); x++) {
			map_data_[y][x] = terrain_data_[y][x];
		}
	}
}

// 
void DungeonManager::spawnPlayer() {

	int area_index = rand() % area_count_;

	int spawn_x = rand() % areas_[area_index].room.width + areas_[area_index].room.x;
	int spawn_y = rand() % areas_[area_index].room.height + areas_[area_index].room.y;

	tnl::DebugTrace("player x = %d, y = %d\n", spawn_x, spawn_y);
	map_data_[spawn_y][spawn_x] = static_cast<int>( eMapData::PLAYER );

}

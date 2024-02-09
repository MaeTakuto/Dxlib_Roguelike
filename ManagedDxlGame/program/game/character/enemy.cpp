#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "enemy.h"


Enemy::Enemy() {

	gpc_hdl_data_ = tnl::LoadCsv("csv/player_gpc_data.csv");

	gpc_hdl_.resize( static_cast<int>( eDir::MAX ) );

	for (int i = 1; i < gpc_hdl_data_.size(); i++) {

		gpc_hdl_[i - 1].resize(gpc_hdl_data_[i][1].getInt());

		gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data_[i][0].getString(),
			gpc_hdl_data_[i][1].getInt(),
			gpc_hdl_data_[i][2].getInt(),
			gpc_hdl_data_[i][3].getInt(),
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	dir_ = eDir::DOWN;
	act_state_ = eActState::IDLE;
	nodes_ = new Node[8];

	is_alive_ = true;
	is_collision_ = false;

}

// =====================================================================================
// 
// =====================================================================================
Enemy::~Enemy() {

	tnl::DebugTrace("Enemyのデストラクタが実行されました。\n");
}

// =====================================================================================
// 敵の初期化（ コンストラクタで初期化できないものの処理 ）
// =====================================================================================
void Enemy::init() {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

	if (scene_play) scene_play_ = scene_play;

}

// =====================================================================================
// 更新
// =====================================================================================
void Enemy::update(float delta_time) {

	sequence_.update(delta_time);
}

// =====================================================================================
// 描画
// =====================================================================================
void Enemy::draw(const std::shared_ptr<Camera> camera) {

	// 描画位置調整
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		gpc_hdl_[static_cast<int>(dir_)][0], true);
}

// =====================================================================================
// 待機シーケンス
// =====================================================================================
bool Enemy::seqIdle(const float delta_time) {

	return true;
}

// =====================================================================================
// next_pos_ に移動させる。
// =====================================================================================
bool Enemy::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_) * MOVE_SPEED;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Enemy::seqIdle);
	}

	return true;
}

// =====================================================================================
// 通路での行動
// =====================================================================================
void Enemy::onRoadAction() {

	auto scene_play = scene_play_.lock();

	if (scene_play == nullptr) return;

	// 行動エラーのカウント回数が3回以上の場合、来た道に方向を向ける
	if (action_error_ >= ACTION_ERROR_MAX) {
		dir_ = getOppositeDir(dir_);
		action_error_ = 0;
	}

	std::vector<eDir> directions;
	//std::vector<eDir> directions = getNearbyMapData( pos_, eMapData::PLAYER );

	//// プレイヤーが隣接していた場合
	//if (directions.size() != 0) {
	//	// 攻撃決定処理
	//	action_error_++;
	//	return;
	//}

	// 地面がある方向を取得する
	directions = getNearbyMapData( pos_, eMapData::GROUND );

	// 来た方向の要素だけ削除する
	auto it = std::find(directions.begin(), directions.end(), getOppositeDir(dir_));
	if (it != directions.end()) directions.erase(it);

	// 移動できる方向がない場合、行動エラーをカウントする
	if (directions.size() == 0) { action_error_++; }
	else if (directions.size() == 1) { setNextPosInDir(directions[0]); }
	// 移動できる方向が複数ある場合、ランダムに選ぶ
	else {
		int index = rand() % directions.size();
		setNextPosInDir(directions[index]);
	}

	changeToMoveAction();

}

// =====================================================================================
// 部屋にいるときの行動
// =====================================================================================
void Enemy::onRoomAction() {

	// ターゲットを見つけていた場合、その方向に向かう
	if (is_target_pos_)	moveToTarget();

	// ターゲットを探す
	else findTargetInRoom();

}

// =====================================================================================
// ターゲットに向かって移動する
// =====================================================================================
void Enemy::moveToTarget() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return;

	// 8方向
	tnl::Vector2i dir[DIR_MAX] = { tnl::Vector2i::up, tnl::Vector2i::down, tnl::Vector2i::left, tnl::Vector2i::right,
								   tnl::Vector2i(-1, -1), tnl::Vector2i(1, -1), tnl::Vector2i(-1, 1), tnl::Vector2i(1, 1) };

	for (int i = 0; i < DIR_MAX; ++i) {
		nodes_[i].pos_ = tnl::Vector2i(pos_.x, pos_.y) + dir[i];

		// マスのコストを設定する。
		nodes_[i].cost_ = abs(target_pos_.x - nodes_[i].pos_.x) + abs(target_pos_.y - nodes_[i].pos_.y);
		nodes_[i].is_enable_ = false;

		if (!isEnableMapPosition(nodes_[i].pos_)) continue;

		// 斜めのセルの確認
		if ( (nodes_[i].pos_.x - pos_.x) != 0 && (nodes_[i].pos_.y - pos_.y) != 0) {

			if (scene_play->getMapData(tnl::Vector3(nodes_[i].pos_.x, nodes_[i].pos_.y, 0)) != eMapData::GROUND) continue;;
			if ( !isEnableMapPosition( tnl::Vector2i((pos_.x + dir[i].x), pos_.y) ) || scene_play->getMapData( tnl::Vector3(pos_.x + dir[i].x, pos_.y, 0) ) == eMapData::WALL ) continue;
			if ( !isEnableMapPosition( tnl::Vector2i(pos_.x, (pos_.y + dir[i].y)) ) || scene_play->getMapData( tnl::Vector3(pos_.x, pos_.y + dir[i].y, 0) ) == eMapData::WALL ) continue;
			nodes_[i].is_enable_ = true;
			continue;
		}

		if(scene_play->getMapData(tnl::Vector3(nodes_[i].pos_.x, nodes_[i].pos_.y, 0)) == eMapData::GROUND) nodes_[i].is_enable_ = true;
	}

	Node* next_node = getMinimunScoreNodeForEnabled();
	next_pos_ = tnl::Vector3(next_node->pos_.x, next_node->pos_.y, 0);

	// 方向の設定
	if((next_pos_.y - pos_.y) > 0 ) dir_ = eDir::DOWN;
	else if ((next_pos_.y - pos_.y) < 0) dir_ = eDir::UP;
	else if ((next_pos_.x - pos_.x) > 0) dir_ = eDir::RIGHT;
	else if ((next_pos_.x - pos_.x) < 0) dir_ = eDir::LEFT;

	if (scene_play->getPlace(next_pos_) == ePlace::ROAD) is_target_pos_ = false;
	changeToMoveAction();
}

// =====================================================================================
// ターゲットを探し、moveToTarget() を実行する。
// =====================================================================================
Node* Enemy::getMinimunScoreNodeForEnabled() {
	Node* p = nullptr;

	for (int i = 0; i < DIR_MAX; ++i) {
		if (nodes_[i].is_enable_ == false) continue;
		if (nullptr == p) p = &nodes_[i];
		if (p->cost_ > nodes_[i].cost_) p = &nodes_[i];
	}

	return p;

}

// =====================================================================================
// ターゲットを探し、moveToTarget() を実行する。
// =====================================================================================
void Enemy::findTargetInRoom() {

	auto scene_play = scene_play_.lock();

	if (scene_play == nullptr) return;

	std::vector<Entrance> entrance;

	int area_id = scene_play->getAreaId(pos_);

	entrance = scene_play->getRoomEntrance(area_id);

	if (entrance.size() == 0) {
		tnl::DebugTrace("vectorが空です\n");
		changeToMoveAction();
		return;
	}

	int target_pos_index = rand() % entrance.size();

	target_pos_ = entrance[target_pos_index].pos;
	target_entrance_id_ = entrance[target_pos_index].id;

	is_target_pos_ = true;

	moveToTarget();
}
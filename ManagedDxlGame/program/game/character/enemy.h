#pragma once
#include "../manager/gm_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"


class Node {
public:
	tnl::Vector2i pos_ = { 0, 0 };
	int cost_ = 0;
	bool is_enable_ = true;

};

class Enemy : public Character {
public:
	Enemy();
	~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	const int ACTION_ERROR_MAX = 3;

	// ScenePlayのインスタンス
	std::weak_ptr<ScenePlay> scene_play_;

	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	tnl::Vector3 target_pos_ = { 0, 0, 0 };

	Node* nodes_ = nullptr;

	bool is_target_pos_ = false;

	int action_error_ = 0;
	int target_entrance_id_ = 0;

	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);

public:

	void init();

	// ゲッター、セッター
	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	inline const tnl::Vector3& getNextPos() override { return next_pos_; }
	inline bool isAlive() override { return is_alive_; }
	inline const eActState& getActState() override { return act_state_; }

	// 
	inline void collisionProcess() override {

	}

	// ダメージを受ける
	inline void takeDamage(int damage) override {}

	// 死亡判定にする
	inline void death() { 
		is_alive_ = false;
		is_target_pos_ = false;
		action_error_ = 0;
	}

	// 行動を決める
	inline void desideAction() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		if (scene_play->getPlace(pos_) == ePlace::ROAD) onRoadAction();
		else onRoomAction();
	}

	// 自身をスポーンさせる
	inline void spawn(const tnl::Vector3& pos) {

		is_alive_ = true;
		pos_ = pos;
		next_pos_ = pos;

		dir_ = eDir::DOWN;
		act_state_ = eActState::IDLE;
	}

private:
	void onRoadAction();

	void onRoomAction();

	void moveToTarget();
	void findTargetInRoom();
	Node* getMinimunScoreNodeForEnabled();

	bool isEnableMapPosition(const tnl::Vector2i& pos) {

		if ( pos.x < 0 ) return false;
		if ( pos.y < 0 ) return false;
		if ( pos.x > GameManager::FIELD_WIDTH ) return false;
		if ( pos.y > GameManager::FIELD_HEIGHT ) return false;
		return true;
	}

	// =====================================================================================
	// 向いている方向を反対を取得する
	// =====================================================================================
	inline eDir getOppositeDir(eDir dir) {

		if (dir == eDir::UP)			return eDir::DOWN;
		else if (dir == eDir::DOWN)		return eDir::UP;
		else if (dir == eDir::LEFT)		return eDir::RIGHT;
		else if (dir == eDir::RIGHT)	return eDir::LEFT;
	}

	// =====================================================================================
	// 指定した方向の１マス先を next_pos_ にセット
	// =====================================================================================
	inline void setNextPosInDir(eDir dir) {

		switch (dir) {
		case eDir::UP:
			next_pos_.y -= 1;
			dir_ = dir;
			break;
		case eDir::DOWN:
			next_pos_.y += 1;
			dir_ = dir;
			break;
		case eDir::LEFT:
			next_pos_.x -= 1;
			dir_ = dir;
			break;
		case eDir::RIGHT:
			next_pos_.x += 1;
			dir_ = dir;
			break;
		default:
			return;
			break;
		}
	}

	// =====================================================================================
	// 指定した地形データを、指定した位置の周りの4方向から探し、存在する方向を返す。
	// =====================================================================================
	inline std::vector<eDir> getNearbyMapData(const tnl::Vector3& pos, eMapData map_data) {
		
		auto scene_play = scene_play_.lock();		
		std::vector<eDir> directions;

		if (scene_play == nullptr) return directions;

		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y - 1, pos.z)) == map_data) directions.emplace_back(eDir::UP);
		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y + 1, pos.z)) == map_data) directions.emplace_back(eDir::DOWN);
		if (scene_play->getMapData(tnl::Vector3(pos.x - 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir::LEFT);
		if (scene_play->getMapData(tnl::Vector3(pos.x + 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir::RIGHT);

		return directions;
	}

	// =====================================================================================
	// 移動シーケンスに切り替える。
	// =====================================================================================
	inline void changeToMoveAction() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
		scene_play->setMapData(next_pos_, eMapData::ENEMY);
		sequence_.change(&Enemy::seqMove);
		act_state_ = eActState::MOVE;
	}

};
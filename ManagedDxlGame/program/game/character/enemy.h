#pragma once
#include "../base/character_base.h"
#include "../scene/scene_play.h"


class Enemy : public Character {
public:
	Enemy();
	~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	const int ACTION_ERROR_MAX = 3;

	// ScenePlay�̃C���X�^���X
	std::weak_ptr<ScenePlay> scene_play_;

	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	tnl::Vector3 target_pos_ = { 0, 0, 0 };

	bool is_target_pos_ = false;

	int action_error_ = 0;
	int target_entrance_id_ = 0;

	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);

public:

	void init();

	// 
	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }

	inline const tnl::Vector3& getNextPos() override { return next_pos_; }

	inline bool isAlive() override { return is_alive_; }

	inline const eActState& getActState() override { return act_state_; }

	inline void collisionProcess() override {

	}

	inline void death() { 
		is_alive_ = false;
		is_target_pos_ = false;
		action_error_ = 0;
	}

	// �s�������߂�
	inline void desideAction() {

		auto scene_play = scene_play_.lock();

		if (scene_play == nullptr) return;

		if (scene_play->getPlace(pos_) == ePlace::ROAD) {
			onRoadAction();
		}
		else {
			onRoomAction();
		}
	}

	// ���g���X�|�[��������
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

	// =====================================================================================
	// �����Ă�������𔽑΂��擾����
	// =====================================================================================
	inline eDir getOppositeDir(eDir dir) {

		if (dir == eDir::UP)			return eDir::DOWN;
		else if (dir == eDir::DOWN)	    return eDir::UP;
		else if (dir == eDir::LEFT)		return eDir::RIGHT;
		else if (dir == eDir::RIGHT)	return eDir::LEFT;
	}

	// =====================================================================================
	// �w�肵�������̂P�}�X��� next_pos_ �ɃZ�b�g
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
	// �w�肵���n�`�f�[�^���A�w�肵���ʒu�̎����4��������T���A���݂��������Ԃ��B
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
	// �ړ��V�[�P���X�ɐ؂�ւ���B
	// =====================================================================================
	inline void changeToMoveAction() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		sequence_.change(&Enemy::seqMove);
		act_state_ = eActState::MOVE;
		scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
		scene_play->setMapData(next_pos_, eMapData::ENEMY);
	}

};
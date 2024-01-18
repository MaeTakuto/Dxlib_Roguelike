#pragma once
#include "../base/character_base.h"


class Enemy : public Character {
public:
	Enemy();
	~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }

	inline const tnl::Vector3& getNextPos() override { return next_pos_; }

	inline bool isAlive() override { return is_alive_; }

	inline const eActState& getActState() override { return act_state_; }
	
	inline void collisionProcess() override {

	}

	inline void death() { is_alive_ = false; }

	inline void spawn(const tnl::Vector3& pos) {

		is_alive_ = true;
		pos_ = pos;
		next_pos_ = pos;

		dir_ = eDir::DOWN;
		act_state_ = eActState::IDLE;
	}

private:
	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	bool seqIdle(const float delta_time);

};
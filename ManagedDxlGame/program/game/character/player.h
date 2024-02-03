#pragma once
#include "../base/character_base.h"

class Camera;


class Player : public Character {
public:
	Player();
	~Player();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) override { pos_ = pos; next_pos_ = pos_; }

	inline const tnl::Vector3& getNextPos() override { return next_pos_; }
	
	inline bool isAlive() { return is_alive_; }

	inline const eActState& getActState() override { return act_state_; }

	inline void beginAct() { act_state_ = eActState::IDLE; };

	// è’ìÀèàóù
	inline void collisionProcess() override {

		act_state_ = eActState::IDLE;
		is_collision_ = false;
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
	}

private:

	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::seqIdle);

	bool seqIdle(const float delta_time);
	bool seqCheckCollision(const float delta_time);
	bool seqActionStundby(const float delta_time);
	bool seqMove(const float delta_time);

};
#pragma once
#include "../base/character_base.h"

class Camera;


class Player : public Character {
public:
	Player();
	~Player();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

	inline const tnl::Vector3& getPos() override { return pos_; }
	inline const tnl::Vector3& getNextPos() override { return next_pos_; }
	inline void setPos(tnl::Vector3& pos) override { pos_ = pos; next_pos_ = pos_; }
	inline const eActState& getActState() override { return act_state_; }

	inline void beginAct() { act_state_ = eActState::IDLE; };

	// è’ìÀèàóù
	inline void collisionProcess() override {

		act_state_ = eActState::IDLE;
		is_collision_ = false;
		// sequence_.undo();
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
		
	}

private:
	std::vector< std::vector<int> > gpc_hdl_;
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;

	tnl::Vector3 pos_ = { 0, 0, 0 };
	tnl::Vector3 next_pos_ = { 0, 0, 0 };

	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::seqIdle);

	bool seqIdle(const float delta_time);
	bool seqCheckCollision(const float delta_time);
	bool seqMove(const float delta_time);

};
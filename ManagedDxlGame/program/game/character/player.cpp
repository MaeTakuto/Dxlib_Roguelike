#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../common/camera.h"
#include "../manager/resource_manager.h"
#include "player.h"


Player::Player() {

	gpc_hdl_data_ = tnl::LoadCsv("csv/player_gpc_data.csv");

	gpc_hdl_.resize(4);

	for (int i = 1; i < gpc_hdl_data_.size(); i++) {
		
		gpc_hdl_[i - 1].resize( gpc_hdl_data_[i][1].getInt() );

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
	is_collision_ = false;

}

Player::~Player() {

}

void Player::update(float delta_time) {

	sequence_.update(delta_time);
}

void Player::draw(std::shared_ptr<Camera> camera) {

	// •`‰æˆÊ’u’²®
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		gpc_hdl_[static_cast<int>(dir_)][0], true);
}

bool Player::seqIdle(const float delta_time) {

	if (act_state_ != eActState::IDLE) return true;

	// ˆÚ“®“ü—Í
	if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_UP)) {
		dir_ = eDir::UP;
		act_state_ = eActState::MOVE;
		next_pos_.y += -1;
		sequence_.change(&Player::seqMove);
		return true;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_S, eKeys::KB_DOWN))	{
		dir_ = eDir::DOWN;
		act_state_ = eActState::MOVE;
		next_pos_.y +=  1;
		sequence_.change(&Player::seqMove);
		return true;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_A, eKeys::KB_LEFT))	{ 
		dir_ = eDir::LEFT;
		act_state_ = eActState::MOVE;
		next_pos_.x += -1;
		sequence_.change(&Player::seqMove);
		return true;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_D, eKeys::KB_RIGHT)) {
		dir_ = eDir::RIGHT;
		act_state_ = eActState::MOVE;
		next_pos_.x +=  1;
		sequence_.change(&Player::seqMove);
		return true;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
		act_state_ = eActState::MOVE;
		sequence_.change(&Player::seqMove);
	}

	return true;
}

// Õ“Ë”»’è‚ÌŠm”F
bool Player::seqCheckCollision(const float delta_time) {

	if ( is_collision_ ) {
		act_state_ = eActState::IDLE;
		is_collision_ = false;
		sequence_.change(&Player::seqIdle);
		next_pos_ = pos_;
		return true;
	}
	else {
		sequence_.change(&Player::seqMove);
		return true;
	}

	return true;
}

// 
bool Player::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_ ) * MOVE_SPEED;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Player::seqIdle);
	}
	//if (abs(next_pos_.x - pos_.x) > 0.1f) {
	//	pos_.x += MOVE_SPEED;
	//}

	return true;
}

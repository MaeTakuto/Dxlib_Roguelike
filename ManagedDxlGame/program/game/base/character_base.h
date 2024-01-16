#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

// ƒLƒƒƒ‰‚Ìs“®ó‘Ô
enum class eActState {
	IDLE,
	ACT,
	ATTACK,
	END
};

class Character {
public:
	Character() {};
	virtual ~Character() {};

	virtual const tnl::Vector3& getPos() = 0;
	virtual const tnl::Vector3& getNextPos() = 0;
	virtual void setPos(tnl::Vector3&) = 0;
	virtual const eActState& getActState() = 0;
	virtual void collisionProcess() = 0;

protected:
	const float MOVE_SPEED = 0.25f;

	std::vector<int> gpc_hdl_;

	eActState act_state_;

	tnl::Vector3 pos_;

	bool is_collision_;


};
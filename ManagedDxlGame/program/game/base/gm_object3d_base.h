#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


class GameObject3D {
public:
	~GameObject3D() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(dxe::Camera* cammera) = 0;

protected:
	tnl::Vector3 root_pos_;
	tnl::Vector3 root_scl_;
	tnl::Quaternion root_rot_;

};
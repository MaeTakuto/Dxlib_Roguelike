#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Camera;

// 2D�Q�[���I�u�W�F�N�g�̊�{�I�ȋ@�\
class GameObject2D {
public:
	~GameObject2D() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const Camera* camera) = 0;

	virtual tnl::Vector3& getPos() = 0;

protected:
	tnl::Vector3 pos_;

	int width_;
	int height_;

};
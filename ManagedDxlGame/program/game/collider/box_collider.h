#pragma once


// �{�b�N�X�^�̏Փ˔���I�u�W�F�N�g
class BoxCollider {
public:
	BoxCollider();
	~BoxCollider();

	inline const tnl::Vector3 getPos() { return center_pos_; }

private:
	tnl::Vector3 center_pos_;

};
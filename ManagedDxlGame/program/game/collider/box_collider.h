#pragma once


// ボックス型の衝突判定オブジェクト
class BoxCollider {
public:
	BoxCollider();
	~BoxCollider();

	inline const tnl::Vector3 getPos() { return center_pos_; }

private:
	tnl::Vector3 center_pos_;

};
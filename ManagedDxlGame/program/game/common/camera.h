#pragma once
#include "../manager/gm_manager.h"


class Camera {
public:
	Camera(const tnl::Vector3& target = { 0, 0, 0 });
	~Camera();

	void update(const tnl::Vector3& target = { 0, 0, 0 });
	void control();

	inline const tnl::Vector3& getPos() { return pos_; }
	inline void setPos(const tnl::Vector3& target) { pos_ = tnl::Vector3(target.x * GameManager::DRAW_CHIP_SIZE, target.y * GameManager::DRAW_CHIP_SIZE, 0)/* + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0)*/; }

private:
	tnl::Vector3 pos_;
	tnl::Vector3 target_;

};
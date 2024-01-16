#include "../../dxlib_ext/dxlib_ext.h"
#include "camera.h"



Camera::Camera(const tnl::Vector3& target) {

	pos_ = { target.x + (DXE_WINDOW_WIDTH >> 1), target.y + (DXE_WINDOW_HEIGHT >> 1), 0 };
}

Camera::~Camera() {

}

void Camera::update(const tnl::Vector3& target) {

	pos_ = tnl::Vector3( target.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0) + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
}

void Camera::control() {

	if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_UP)) {
		pos_.y -= 10;
		//if (pos_.y < TOP_POS_Y) pos_.y = TOP_POS_Y;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_S, eKeys::KB_DOWN)) {
		pos_.y += 10;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_A, eKeys::KB_LEFT)) {
		pos_.x -= 10;
		//if (pos_.x < TOP_POS_X) pos_.x = TOP_POS_X;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_D, eKeys::KB_RIGHT)) {
		pos_.x += 10;
	}
}
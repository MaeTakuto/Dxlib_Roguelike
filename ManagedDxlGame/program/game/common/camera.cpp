#include "../../dxlib_ext/dxlib_ext.h"
#include "camera.h"



Camera::Camera(const tnl::Vector3& target = { 0, 0, 0 }) {

	pos_ = { target.x + (DXE_WINDOW_WIDTH >> 1), target.y + (DXE_WINDOW_HEIGHT >> 1), 0 };
}

Camera::~Camera() {

}

void Camera::update(const tnl::Vector3& target = { 0, 0, 0 }) {

	pos_ = { target.x + (DXE_WINDOW_WIDTH >> 1), target.y + (DXE_WINDOW_HEIGHT >> 1), 0 };
}
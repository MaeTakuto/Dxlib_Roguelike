#pragma once


class Camera {
public:
	Camera(const tnl::Vector3& target);
	~Camera();

	void update(const tnl::Vector3& target);

private:
	tnl::Vector3 pos_;

};
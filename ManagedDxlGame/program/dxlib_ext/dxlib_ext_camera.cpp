#include "dxlib_ext_camera.h"

namespace dxe {

	void Camera::render( float scale, uint32_t color ) {

		// オブジェクトのワールド行列の作成
		tnl::Matrix mt_trans;
		tnl::Matrix mt_rot;
		tnl::Matrix mt_obj_world;
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);

		tnl::Quaternion rot = tnl::Quaternion::LookAt(pos_, target_, up_);
		mt_rot = rot.getMatrix();
		MATRIX im;
		mt_obj_world = mt_rot * mt_trans;
		memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
		SetTransformToWorld(&im);

		float w = 10 * 0.5f * scale;
		float h = 10 * 0.5f * scale;
		float d = 20 * 0.5f * scale;
		{
			tnl::Vector3 v[8] = {
				{ -w,  h,  d }, {  w,  h,  d },  {  w,  h,  -d }, { -w,  h, -d },
				{ -w, -h,  d }, {  w, -h,  d },  {  w, -h,  -d }, { -w, -h, -d }
			};
			uint32_t col = color;
			for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
			for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
			for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
			DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
			DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);
		}

		float ds = 5 * scale;
		float d2 = d + ds ;
		float d3 = d ;
		float ww = w / 2;
		float hh = h / 2;
		{
			tnl::Vector3 v[8] = {
				{ -w,  h,  d2 }, {  w,  h,  d2 },  {  ww,  hh,  d3 }, { -ww,  hh, d3 },
				{ -w, -h,  d2 }, {  w, -h,  d2 },  {  ww, -hh,  d3 }, { -ww, -hh, d3 }
			};
			uint32_t col = color;
			for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
			for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
			for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
			DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
			DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);
		}

	}

}
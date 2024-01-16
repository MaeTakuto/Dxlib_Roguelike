#pragma once

#include "../library/tnl_util.h"
#include "../library/tnl_using.h"
#include "dxlib_ext_gui_value_slider.h"

namespace dxe {

	class Mesh;
	class MeshMtrlGuiController {
	public :
		MeshMtrlGuiController( Mesh* ref_mesh );
		void update();
		void draw();
	private :
		Mesh* ref_mesh_ = nullptr;
		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<GuiValueSlider< Mesh, tnl::Vector3 >> diff_;
		Shared<GuiValueSlider< Mesh, tnl::Vector3 >> ambi_;
		Shared<GuiValueSlider< Mesh, tnl::Vector3 >> emis_;
		Shared<GuiValueSlider< Mesh, tnl::Vector3 >> spec_;
		Shared<GuiValueSlider< Mesh, float >> spec_pow_;
	};

}


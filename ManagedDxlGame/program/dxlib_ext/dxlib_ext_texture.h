#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include "DxLib.h"
namespace dxe {

	class Texture final {
	public :
		~Texture() {
			if(graph_hdl_) DeleteGraph(graph_hdl_);
			if (file_data_) delete[] file_data_;
		}

		// 幅の取得
		uint32_t getWidth() { return width_; }

		// 高さの取得
		uint32_t getHeight() { return height_; }

		// ファイルパスの取得
		const std::string& getFilePath() { return file_path_; }

		// ファイル名の取得
		const std::string& getFileName() { return file_name_; }

		// グラフィックハンドルの取得
		int getDxLibGraphHandle() { return graph_hdl_; }

		// データサイズの取得
		uint32_t getDataSize() { return data_size_; }

		// ファイルデータの取得
		// tips... ピクセルデータではありません
		char* getFileData() { return file_data_; }

		// 生成
		// arg1... ファイルパス
		static std::shared_ptr<Texture> CreateFromFile( const std::string& file_path );
		static std::shared_ptr<Texture> CreateFromMemory( char* file_data, uint32_t data_size, const std::string& regist_key_ = "" );
		static std::shared_ptr<Texture> CreateFromColorBufferRGBA8( unsigned char* color_buffer_rgba8, uint32_t width, uint32_t height);
		static std::shared_ptr<Texture> CreateFromDxLibGraphHdl( int dxlib_graph_hdl );

		static void DestroyUnReferenceTextures();

	private:
		friend class Mesh;
		Texture() {}
		int graph_hdl_ = 0;
		uint32_t width_ = 0;
		uint32_t height_ = 0;
		uint32_t data_size_ = 0;
		std::string file_path_;
		std::string file_name_;
		char* file_data_ = nullptr;
		static std::unordered_map<size_t, Shared<dxe::Texture>> managed_map_;
	};

}


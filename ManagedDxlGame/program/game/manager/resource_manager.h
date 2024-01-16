#pragma once



// 画像、アニメーション、サウンドのパスを管理するクラス
class ResourceManager {
public:
	// インスタンスを返す
	static ResourceManager* getInstance();

	// インスタンスを削除
	static void destroy();

	// 画像のロード、画像のアドレスを渡す。
	int loadGraph(const std::string& gpc_hdl_path);

	// 画像の削除
	void deleteGraph(const std::string& gpc_hdl_path);

	// アニメーションのロード
	std::vector<int>& loadAnimation(const std::string& anim_hdl_path,
		int all_size, int x_size, int y_size, int anim_hdl_width, int anim_hdl_height);

	// アニメーションの削除
	void deleteAnimation(const std::string& anim_hdl_path, int all_size);

	// サウンドのアドレスを渡す。
	int loadSound(const std::string& snd_path);

	// サウンドの削除
	void deleteSound(const std::string& snd_path);

private:
	ResourceManager();
	~ResourceManager();

	// 画像データの格納用コンテナ
	std::unordered_map< std::string, int > gpc_hdl_container_;

	// アニメーションデータの格納用コンテナ
	std::unordered_map< std::string, std::vector<int> >
		anim_hdl_container_;

	// サウンドデータの格納用コンテナ
	std::unordered_map< std::string, int > snd_container_;

};
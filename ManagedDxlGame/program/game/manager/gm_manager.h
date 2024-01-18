#pragma once

class SceneBase;

class GameManager {
public:
	// デストラクタ
	~GameManager();

	// ====== const ======
	static constexpr int CHIP_SIZE = 32;									// 画像チップサイズ
	static constexpr int DRAW_CHIP_SIZE = CHIP_SIZE / 2;					// オブジェクトの表示サイズ

	// ゲームマネージャーのアドレスを返す
	static GameManager* GetInstance(std::shared_ptr<SceneBase> = nullptr);

	// アドレスの削除
	inline static void Destroy() { delete GetInstance(); }

	// ゲームマネージャーの更新
	void update(float delta_time);

	// シーンの切り替えを行う
	void changeScene(std::shared_ptr<SceneBase> next_scene, float fade_time = 2.0f);

	// シーンのインスタンスを返す。
	inline std::shared_ptr<SceneBase> getSceneInstance() { return now_scene_; }	

	// フェード演出フラグを返す。
	inline bool isTransition() { return is_transition_; }

private:
	// コンストラクタ
	GameManager(std::shared_ptr<SceneBase> start_scene);

	// 現在のシーン
	std::shared_ptr<SceneBase> now_scene_ = nullptr;

	// 次のシーン
	std::shared_ptr<SceneBase> next_scene_ = nullptr;

	std::string transition_gpc_hdl_path_;									// フェード演出画像パス

	int transition_gpc_hdl_;												// フェード演出画像
	float fade_time_ = 0.0f;												// フェード演出時間
	float fade_time_wait_ = 0.25f;											// フェード演出待機時間
	bool is_transition_ = false;											// フェード演出フラグ

	// ゲームマネージャーのシーケンス
	tnl::Sequence<GameManager> sequence_
		= tnl::Sequence<GameManager>(this, &GameManager::seqRun);

	// 各シーケンス
	bool seqRun(const float delta_time);									// 何もしない
	bool seqTransOut(const float delta_time);								// フェードアウト
	bool seqWait(const float delta_time);									// フェード演出待機
	bool seqTransIn(const float delta_time);								// フェードイン

};
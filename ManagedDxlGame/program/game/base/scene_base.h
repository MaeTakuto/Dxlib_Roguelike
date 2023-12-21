#pragma once


class SceneBase {
public:
	virtual ~SceneBase() {};

	// シーン遷移の処理が完了しているかを返す。
	virtual bool isTransitionProcess() = 0;

	// 更新
	virtual void update(float delta_time) = 0;

	// 描画
	virtual void draw() = 0;

protected:
	// シーン遷移処理完了の確認用
	bool is_transition_process_;

};
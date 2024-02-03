#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../scene/scene_play.h"
#include "../character/enemy.h"

class Camera;
class Enemy;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int ENEMY_MAX = 10;
	
	std::vector< std::shared_ptr<Enemy> > enemys_;

public:
	// 
	inline void init() {

		for (int i = 0; i < enemys_.size(); i++) {
			enemys_[i]->init();
		}
	}

	// 敵をスポーンさせる
	inline void spawnEnemy(const tnl::Vector3& pos) {

		for (int i = 0; i < enemys_.size(); i++) {
			if (enemys_[i]->isAlive() == false) {
				enemys_[i]->spawn(pos);
				return;
			}
		}
	}

	// 全ての敵をデスにする
	inline void deathAllEnemys() {

		for (int i = 0; i < enemys_.size(); i++) {
			enemys_[i]->death();
		}
	}

	// 敵の行動を決定する
	inline void beginAction() {

		for (int i = 0; i < enemys_.size(); i++) {
			// tnl::DebugTrace("%d\n", i);
			if (enemys_[i]->isAlive() == false) continue;
			enemys_[i]->desideAction();
		}
	}
	
	// 全ての敵が行動終了しているかを判定する
	inline bool isAllEnemyActEnd() {

		for (int i = 0; i < enemys_.size(); i++) {
			if (enemys_[i]->isAlive() == false) continue;
			if (enemys_[i]->getActState() != eActState::END) return false;
		}

		return true;
	}

};
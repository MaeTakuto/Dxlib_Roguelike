#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../character/enemy.h"

class Camera;
class Enemy;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

	// 敵をスポーンさせる
	inline void spawnEnemy(const tnl::Vector3& pos) {
		
		for (int i = 0; i < enemys.size(); i++) {
			if (enemys[i]->isAlive() == false) {
				enemys[i]->spawn(pos);
				return;
			}
		}
	}

	// 全ての敵をデスにする。
	inline void deathAllEnemys() {
		
		for (int i = 0; i < enemys.size(); i++) {
			enemys[i]->death();
		}
	}

private:
	const int ENEMY_MAX = 10;

	std::vector< std::shared_ptr<Enemy> > enemys;

};
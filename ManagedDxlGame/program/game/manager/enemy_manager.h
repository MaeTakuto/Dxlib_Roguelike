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

	// �G���X�|�[��������
	inline void spawnEnemy(const tnl::Vector3& pos) {
		
		for (int i = 0; i < enemys.size(); i++) {
			if (enemys[i]->isAlive() == false) {
				enemys[i]->spawn(pos);
				return;
			}
		}
	}

	// �S�Ă̓G���f�X�ɂ���B
	inline void deathAllEnemys() {
		
		for (int i = 0; i < enemys.size(); i++) {
			enemys[i]->death();
		}
	}

private:
	const int ENEMY_MAX = 10;

	std::vector< std::shared_ptr<Enemy> > enemys;

};
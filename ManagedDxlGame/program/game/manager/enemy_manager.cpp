#include "../../dxlib_ext/dxlib_ext.h"
#include "../character/enemy.h"
#include "enemy_manager.h"


EnemyManager::EnemyManager() {

	enemys.resize(ENEMY_MAX);

	for (int i = 0; i < enemys.size(); i++) {

		enemys[i] = std::make_shared<Enemy>();
		enemys[i]->setPos(tnl::Vector3{ 0, 0, 0 });
		enemys[i]->death();
	}

}

// デストラクタ
EnemyManager::~EnemyManager() {

}

// 
void EnemyManager::update(float delta_time) {

	for (int i = 0; i < enemys.size(); i++) {
		if (enemys[i]->isAlive())
			enemys[i]->update(delta_time);
	}
}

// 
void EnemyManager::draw(const std::shared_ptr<Camera> camera) {

	for (int i = 0; i < enemys.size(); i++) {
		if (enemys[i]->isAlive())
			enemys[i]->draw(camera);
	}

}

#include "../../dxlib_ext/dxlib_ext.h"
#include "../character/enemy.h"
#include "enemy_manager.h"


EnemyManager::EnemyManager() {

	enemys_.resize(ENEMY_MAX);

	for (int i = 0; i < enemys_.size(); i++) {

		enemys_[i] = std::make_shared<Enemy>();
		enemys_[i]->setPos(tnl::Vector3{ 0, 0, 0 });
		enemys_[i]->death();
	}

}

// デストラクタ
EnemyManager::~EnemyManager() {

	tnl::DebugTrace("EnemyManagerのデストラクタが実行されました\n");

	enemys_.clear();
}

// 
void EnemyManager::update(float delta_time) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->isAlive())
			enemys_[i]->update(delta_time);
	}
}

// 
void EnemyManager::draw(const std::shared_ptr<Camera> camera) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->isAlive())
			enemys_[i]->draw(camera);
	}

}

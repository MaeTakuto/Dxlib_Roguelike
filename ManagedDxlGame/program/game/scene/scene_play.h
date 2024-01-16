#pragma onc
#include "../base/scene_base.h"

class DungeonManager;
class Camera;
class Player;

class ScenePlay : public SceneBase {
public:
	ScenePlay();
	~ScenePlay();

	inline bool isTransitionProcess() override { return is_transition_process_; }

	void update(float delta_time) override;
	void draw() override;

private:
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqGenerateDungeon);
	tnl::Sequence<ScenePlay> in_dungeon_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqPlayerAct);

	std::shared_ptr<DungeonManager> dng_generator_ = nullptr;
	std::shared_ptr<Camera> camera_ = nullptr;
	std::shared_ptr<Player> player_ = nullptr;

	// �n�`�f�[�^
	std::vector< std::vector<int> > terrain_data_;
	std::vector< std::vector<int> > map_data_;

	// �}�b�v�`�b�v�̉摜�i�[
	std::vector<int> mapchip_gpc_hdl_;
	int mapchip_all_size_ = 2;

	// ========= �f�o�b�O ============
	void debugMapData() {

		tnl::DebugTrace("============== map_data_ ===============\n");

		for (int y = 0; y < map_data_.size(); y++) {
			for (int x = 0; x < map_data_[y].size(); x++) {
				tnl::DebugTrace("%d", map_data_[y][x]);
			}
			tnl::DebugTrace("\n");
		}
	}

	// �L�����N�^�[�A�b�v�f�[�g
	void charaUpdate(float delta_time);

	// =========== ���C���V�[�P���X ===========
	// bool seqFadeIn(const float delta_time);
	bool seqGenerateDungeon(const float delta_time);
	// bool seqFadeOut(const float delta_time);
	bool seqMain(const float delta_time);

	// =========== �_���W�����T�����̃V�[�P���X ===========
	bool seqPlayerAct(const float delta_time);
	bool seqCharaMove(const float delta_time);
	bool seqActEndProcess(const float delta_time);

	// void charaAnimUpdate(float delta_time);

	// ==============================================
	//				�C�����C���֐�
	// ==============================================

	// �w�肳�ꂽ���W�̃}�b�v�f�[�^��Ԃ�
	inline int getMapData(const tnl::Vector3& pos) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return map_data_[y][x];
	}

};
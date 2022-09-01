#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"input.h"
#include"DebugText.h"
#include"EnemyBullet.h"
#include<memory>
#include<list>

//���@�N���X�̑O���錾
class Player;


//�s���t�F�[�Y
enum class Phase {
	Approach,	//�ڋ߂���
	Leave,		//���E����
};

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ڋ߃t�F�[�Y�̏���������
	/// </summary>
	void InitializeApproach();


	/// <summary>
	/// �ڋ߃t�F�[�Y�̍X�V����
	/// </summary>
	void UpdateApproach();

	/// <summary>
	/// ���E�t�F�[�Y�̍X�V����
	/// </summary>
	void UpdateLeave();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetworldPos();

	//�Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets_; }

	bool GetAlive() { return isAlive; }

	void Spawn(Vector3 pos);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//���L����
	Player* player_ = nullptr;

	//�e�̔��˃^�C�}�[
	int32_t fireTimer = 0;

	bool isAlive = false;

	int spawnInterval = 60;
	int spawnTimer = 0;

public:
	//���ˊԊu
	static const int kFireInterval = 180;
};


#pragma once
#include"Model.h"
#include"WorldTransform.h"

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
};


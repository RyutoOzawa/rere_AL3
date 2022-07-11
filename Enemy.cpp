#include "Enemy.h"
#include<cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle,Vector3 pos)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����f�[�^�������o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = textureHandle;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�������W�Ɉړ�
	worldTransform_.translation_ = pos;

	velocity_ = { 0,0,-1 };
}

void Enemy::Update()
{
	////���x���ړ�
	//worldTransform_.translation_ += velocity_;

	switch (phase_) {
	
	case Phase::Approach:
		UpdateApproach();
		break;
	case Phase::Leave:
		UpdateLeave();
		break;
	default:
		break;
	}

	//�s��̍X�V
	worldTransform_.MatUpdate();
}

void Enemy::UpdateApproach()
{
	//�ڋ߃t�F�[�Y�̑��x
	Vector3 approachVelocity = { 0,0,-1 };
	//�ړ�
	worldTransform_.translation_ += approachVelocity;

	//����̈ʒu�ɓ��B�����痣�E�t�F�[�Y��
	if (worldTransform_.translation_.z < 0) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	Vector3 leaveVelocity = { 0.5f,0.5f,-0.5f };
	//�ړ�
	worldTransform_.translation_ += leaveVelocity;
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

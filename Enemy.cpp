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
	//���x���ړ�
	worldTransform_.translation_ += velocity_;

	//�s��̍X�V
	worldTransform_.MatUpdate();
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

#include "Enemy.h"
#include<cassert>
using namespace MathUtility;

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

	//�ڋ߃t�F�[�Y������
	InitializeApproach();
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

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	//�s��̍X�V
	worldTransform_.MatUpdate();
}

void Enemy::InitializeApproach()
{
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}

void Enemy::UpdateApproach()
{
	//���˃^�C�}�[�J�E���g�_�E��1
	fireTimer--;
	//�w�莞�ԂɒB����
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

	//�ڋ߃t�F�[�Y�̑��x
	Vector3 approachVelocity = { 0,0,-0.25f };
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

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpd = -1.0f;
	Vector3 velocity(0, 0, kBulletSpd);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = Vector3MultiTransform(velocity, worldTransform_.matWorld_);

	//�e�̐����ƒǉ�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	enemyBullets_.push_back(std::move(newBullet));

}

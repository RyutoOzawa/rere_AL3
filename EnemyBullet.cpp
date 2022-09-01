#include "EnemyBullet.h"
#include<cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//null�`�F�b�N
	assert(model);

	model_ = model;
//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");

	//���[���h�ϊ�������
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };

	//�����Ŏ󂯎�������x���Z�b�g
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//���W���ړ�������(1f���̈ړ��ʂ𑫂�)
	worldTransform_.translation_ += velocity_;

	//���[���h�ϊ��X�V
	worldTransform_.MatUpdate();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPos()
{
	Vector3 worldPos;

	worldPos = worldTransform_.translation_;

	return worldPos;
}

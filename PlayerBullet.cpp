#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h�ϊ�������
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//�����Ŏ󂯎�������x���Z�b�g
	velocity_ = velocity;

	worldTransform_.scale_ = { 0.25f,0.25f,0.25f };
}

void PlayerBullet::Update()
{
	//���W���ړ�������(1f���̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	//���[���h�ϊ��X�V
	worldTransform_.MatUpdate();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPos()
{
	Vector3 worldPos;

	worldPos = worldTransform_.translation_;

	return worldPos;
}

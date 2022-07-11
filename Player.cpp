#include "Player.h"
#include<cassert>
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����f�[�^�������o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_.z = 20.0f;
}

void Player::Update()
{
	//�f�o�b�O�p
	if (input_->PushKey(DIK_J)) {
		worldTransform_.scale_.x += 0.05f;
	}
	else 	if (input_->PushKey(DIK_K)) {
		worldTransform_.scale_.x -= 0.05f;
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});


	//���񏈗�
	Rotate();

	//�L�����N�^�[�ړ�
	Vector3 move = { 0,0,0 };
	const float kMoveSpd = 0.5f;

	if (input_->PushKey(DIK_A)) {
		move.x = -kMoveSpd;
	}
	else if (input_->PushKey(DIK_D)) {
		move.x = kMoveSpd;
	}

	if (input_->PushKey(DIK_W)) {
		move.z = kMoveSpd;
	}
	else if (input_->PushKey(DIK_S)) {
		move.z = -kMoveSpd;
	}

	if (input_->PushKey(DIK_Q)) {
		move.y = kMoveSpd;
	}
	else if (input_->PushKey(DIK_E)) {
		move.y = -kMoveSpd;
	}

	worldTransform_.translation_ += move;

	//�ړ����E
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�U������
	Attack();

	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet: bullets_){
		bullet->Update();
	}

	//�s��̍X�V
	worldTransform_.MatUpdate();

	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"pos:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate()
{
	Vector3 rotate{ 0,0,0 };
	if (input_->PushKey(DIK_U)) {
		rotate.y = MathUtility::Radian(1.0f);
	}
	else if (input_->PushKey(DIK_I)) {
		rotate.y = -MathUtility::Radian(1.0f);
	}
	worldTransform_.rotation_ += rotate;
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpd = 1.0f;
		Vector3 velocity(0, 0, kBulletSpd);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = Vector3MultiTransform(velocity, worldTransform_.matWorld_);

		//�e�̐����Ə�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		bullets_.push_back(std::move(newBullet));
	}
}

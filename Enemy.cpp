#include "Enemy.h"
#include"Player.h"
#include<cassert>
#include<random>
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

	velocity_ = { 0,0,-0.25f };

	//�ڋ߃t�F�[�Y������
	InitializeApproach();
}

void Enemy::Update()
{
	////���x���ړ�
	//worldTransform_.translation_ += velocity_;

	if (isAlive) {

		switch (phase_) {

		case Phase::Approach:
			UpdateApproach();
			break;
		case Phase::Leave:
			UpdateLeave();
			/*worldTransform_.translation_.z += 100.0f ;
			phase_ = Phase::Approach;*/
			break;
		default:
			break;
		}
	}
	else if (!isAlive) {
		if (spawnTimer > 0) {
			spawnTimer--;
		}
		else if (spawnTimer == 0) {
			isAlive = true;
			//�����V�[�h������
			std::random_device seed_gen;
			//�����Z���k�E�c�C�X�^�[�̗����G���W��
			std::mt19937_64 engine(seed_gen());

			//���W�p�̗����͈͂̎w��
			std::uniform_real_distribution<float> posDist(-10, 10);
			worldTransform_.translation_ = { posDist(engine),0,100.0f };
			phase_ = Phase::Approach;
		}
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
	Vector3 approachVelocity = { 0,0,-0.15f };
	//�ړ�
	worldTransform_.translation_ += approachVelocity;

	//����̈ʒu�ɓ��B�����痣�E�t�F�[�Y��
	if (worldTransform_.translation_.z < -0) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	Vector3 leaveVelocity = { 0.25f,0.0f,-0.0f };
	if (worldTransform_.translation_.x > 0) {
		leaveVelocity.x = 0.1f;
	}
	else {
		leaveVelocity.x = -0.1f;
	}

	//�ړ�
	worldTransform_.translation_ += leaveVelocity;

	//40�𒴂�������W���Z�b�g
	if (fabs(worldTransform_.translation_.x) > 40) {
		//�����V�[�h������
		std::random_device seed_gen;
		//�����Z���k�E�c�C�X�^�[�̗����G���W��
		std::mt19937_64 engine(seed_gen());

		//���W�p�̗����͈͂̎w��
		std::uniform_real_distribution<float> posDist(-10, 10);
		worldTransform_.translation_ ={ posDist(engine),0,100.0f };
		phase_ = Phase::Approach;
	}
}

void Enemy::Draw(ViewProjection viewProjection)
{
	if (isAlive) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		if (!bullet->IsDead()) {
			bullet->Draw(viewProjection);
		}
	}
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpd = 0.25f;
	// �����x�N�g�����i�[����ϐ�
	Vector3 diffVec;

	// ���L�����A�G�L�����̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPos();
	Vector3 enemyPos = worldTransform_.translation_;

	// �����x�N�g�������߂�
	diffVec = playerPos - enemyPos;

	// �����x�N�g���̐��K��
	diffVec.normalize();

	// �x�N�g���̒������A�����ɍ��킹��B( ���́H )
	diffVec *= kBulletSpd;



	//�e�̐����ƒǉ�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, diffVec);

	enemyBullets_.push_back(std::move(newBullet));

}

Vector3 Enemy::GetworldPos()
{
	Vector3 worldPos;

	worldPos = worldTransform_.translation_;

	return worldPos;
}

void Enemy::OnCollision()
{
	isAlive = false;
	spawnTimer = spawnInterval;
}

void Enemy::Spawn(Vector3 pos)
{
	phase_ = Phase::Approach;
	worldTransform_.translation_ = pos;
	isAlive = true;
}

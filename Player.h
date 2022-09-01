#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"input.h"
#include"DebugText.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>
#include"Reticle.h"

class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	WorldTransform GetWorldTransform() const { return worldTransform_; }

	Vector3 GetWorldPos();

	//�Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���e�B�N��
	Reticle reticle;

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;


};


#include "WorldTransform.h"
#include"MathUtility.h"
#include"GameScene.h"
#include"Matrix4.h"

Matrix4 WorldTransform::Scale()
{
	//�s��̐錾
	Matrix4 matScale;
	matScale.IdentityMatrix();

	//�{�����s��ɐݒ�
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;


	return matScale;
}

Matrix4 WorldTransform::Rotation()
{
	//�e���̉�]�s���錾
	Matrix4 matRot;
	Matrix4 matRotZ;
	Matrix4 matRotX;
	Matrix4 matRotY;

	matRot.IdentityMatrix();
	matRotZ.IdentityMatrix();
	matRotX.IdentityMatrix();
	matRotY.IdentityMatrix();

	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[0][0] = 1.0f;
	matRotX.m[3][3] = 1.0f;

	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;


	return matRot;
}

Matrix4 WorldTransform::Translation()
{

	//���s�ړ��s���錾
	Matrix4 matTrans;

	matTrans.IdentityMatrix();
	//�ړ��ʂ��s��ɐݒ�
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	matTrans.m[3][3] = 1.0f;

	return matTrans;
}

void WorldTransform::MatUpdate()
{
	//���[���h���W�ϊ��ɕK�v�ȍs��̐錾
	Matrix4 matScale;
	Matrix4 matRot;
	Matrix4 matTrans;

	//���W��������
	matScale = Scale();
	matRot = Rotation();
	matTrans = Translation();


	//�P�ʍs�����
	matWorld_.IdentityMatrix();
	//�s����|���Z���đ��
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	//�e�����Ȃ�e�̍s�������Ɍv�Z
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	//�s��̓]��
	TransferMatrix();
}

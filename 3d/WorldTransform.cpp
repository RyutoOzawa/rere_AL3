#include "WorldTransform.h"
#include"MathUtility.h"
#include"GameScene.h"
#include"Matrix4.h"

Matrix4 WorldTransform::Scale()
{
	//行列の宣言
	Matrix4 matScale;
	matScale.IdentityMatrix();

	//倍率を行列に設定
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;


	return matScale;
}

Matrix4 WorldTransform::Rotation()
{
	//各軸の回転行列を宣言
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

	//平行移動行列を宣言
	Matrix4 matTrans;

	matTrans.IdentityMatrix();
	//移動量を行列に設定
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	matTrans.m[3][3] = 1.0f;

	return matTrans;
}

void WorldTransform::MatUpdate()
{
	//ワールド座標変換に必要な行列の宣言
	Matrix4 matScale;
	Matrix4 matRot;
	Matrix4 matTrans;

	//座標をかける
	matScale = Scale();
	matRot = Rotation();
	matTrans = Translation();


	//単位行列を代入
	matWorld_.IdentityMatrix();
	//行列を掛け算して代入
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	//親を持つなら親の行列をさらに計算
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	//行列の転送
	TransferMatrix();
}

#include "Enemy.h"
#include<cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle,Vector3 pos)
{
	//NULLポインタチェック
	assert(model);

	//引数データをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//初期座標に移動
	worldTransform_.translation_ = pos;

	velocity_ = { 0,0,-1 };
}

void Enemy::Update()
{
	////速度分移動
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

	//行列の更新
	worldTransform_.MatUpdate();
}

void Enemy::UpdateApproach()
{
	//接近フェーズの速度
	Vector3 approachVelocity = { 0,0,-1 };
	//移動
	worldTransform_.translation_ += approachVelocity;

	//特定の位置に到達したら離脱フェーズへ
	if (worldTransform_.translation_.z < 0) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	Vector3 leaveVelocity = { 0.5f,0.5f,-0.5f };
	//移動
	worldTransform_.translation_ += leaveVelocity;
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

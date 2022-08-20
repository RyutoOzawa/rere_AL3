#include "Enemy.h"
#include<cassert>
using namespace MathUtility;

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

	//接近フェーズ初期化
	InitializeApproach();
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

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	//行列の更新
	worldTransform_.MatUpdate();
}

void Enemy::InitializeApproach()
{
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

void Enemy::UpdateApproach()
{
	//発射タイマーカウントダウン1
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	//接近フェーズの速度
	Vector3 approachVelocity = { 0,0,-0.25f };
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

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	//弾の速度
	const float kBulletSpd = -1.0f;
	Vector3 velocity(0, 0, kBulletSpd);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = Vector3MultiTransform(velocity, worldTransform_.matWorld_);

	//弾の生成と追加
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	enemyBullets_.push_back(std::move(newBullet));

}

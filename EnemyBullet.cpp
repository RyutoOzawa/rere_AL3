#include "EnemyBullet.h"
#include<cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//nullチェック
	assert(model);

	model_ = model;
//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	//ワールド変換初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };

	//引数で受け取った速度をセット
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//座標を移動させる(1f分の移動量を足す)
	worldTransform_.translation_ += velocity_;

	//ワールド変換更新
	worldTransform_.MatUpdate();

	//時間経過でデス
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

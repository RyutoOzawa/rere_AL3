#include "Enemy.h"
#include"Player.h"
#include<cassert>
#include<random>
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

	velocity_ = { 0,0,-0.25f };

	//接近フェーズ初期化
	InitializeApproach();
}

void Enemy::Update()
{
	////速度分移動
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
			//乱数シード生成器
			std::random_device seed_gen;
			//メルセンヌ・ツイスターの乱数エンジン
			std::mt19937_64 engine(seed_gen());

			//座標用の乱数範囲の指定
			std::uniform_real_distribution<float> posDist(-10, 10);
			worldTransform_.translation_ = { posDist(engine),0,100.0f };
			phase_ = Phase::Approach;
		}
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
	Vector3 approachVelocity = { 0,0,-0.15f };
	//移動
	worldTransform_.translation_ += approachVelocity;

	//特定の位置に到達したら離脱フェーズへ
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

	//移動
	worldTransform_.translation_ += leaveVelocity;

	//40を超えたら座標リセット
	if (fabs(worldTransform_.translation_.x) > 40) {
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());

		//座標用の乱数範囲の指定
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
	//弾の速度
	const float kBulletSpd = 0.25f;
	// 差分ベクトルを格納する変数
	Vector3 diffVec;

	// 自キャラ、敵キャラのワールド座標を取得
	Vector3 playerPos = player_->GetWorldPos();
	Vector3 enemyPos = worldTransform_.translation_;

	// 差分ベクトルを求める
	diffVec = playerPos - enemyPos;

	// 差分ベクトルの正規化
	diffVec.normalize();

	// ベクトルの長さを、速さに合わせる。( ←は？ )
	diffVec *= kBulletSpd;



	//弾の生成と追加
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

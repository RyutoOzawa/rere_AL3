#include "Player.h"
#include<cassert>
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数データをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_.z = 20.0f;
}

void Player::Update()
{
	//デバッグ用
	if (input_->PushKey(DIK_J)) {
		worldTransform_.scale_.x += 0.05f;
	}
	else 	if (input_->PushKey(DIK_K)) {
		worldTransform_.scale_.x -= 0.05f;
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});


	//旋回処理
	Rotate();

	//キャラクター移動
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

	//移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//攻撃処理
	Attack();

	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet: bullets_){
		bullet->Update();
	}

	//行列の更新
	worldTransform_.MatUpdate();

	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"pos:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
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
		//弾の速度
		const float kBulletSpd = 1.0f;
		Vector3 velocity(0, 0, kBulletSpd);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Vector3MultiTransform(velocity, worldTransform_.matWorld_);

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		bullets_.push_back(std::move(newBullet));
	}
}

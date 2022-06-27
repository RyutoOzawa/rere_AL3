#include "Player.h"
#include<cassert>


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
}

void Player::Update()
{
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
		move.y = kMoveSpd;
	}
	else if (input_->PushKey(DIK_S)) {
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
	if (bullet_) {
		bullet_->Update();
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
	if (bullet_) {
		bullet_->Draw(viewProjection);
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
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet;
	}
}

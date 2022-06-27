#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include"MathUtility.h"
#include<random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//回転角用の乱数範囲の指定
	std::uniform_real_distribution<float> rotDist(0, 360);
	//座標用の乱数範囲の指定
	std::uniform_real_distribution<float> posDist(-10, 10);


	//ファイル名を指定してテクスチャを読み込む
	texutureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度設定)
	//viewProjection_.up = { cosf(MathUtility::PI / 4.0f),sinf(MathUtility::PI / 4.0f),0.0f };

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = MathUtility::Radian(10);

	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
//	viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
//	viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());


	//for (WorldTransform& worldTransform : worldTransforms_) {

	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();

	//	worldTransform.scale_ = { 1,1,1 };
	//	worldTransform.Scale();
	//	worldTransform.rotation_ ={ rotDist(engine),rotDist(engine) ,rotDist(engine) };
	//	worldTransform.Rotation();
	//	worldTransform.translation_ = { posDist(engine),posDist(engine),posDist(engine) };
	//	worldTransform.Translation();
	//}

	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	//脊髄
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,0,0 };
	//上半身
	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0,2.5f,0 };
	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0,2.5f,0 };
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -2.5f,0,0 };
	worldTransforms_[PartId::kArmL2].Initialize();
	worldTransforms_[PartId::kArmL2].parent_ = &worldTransforms_[PartId::kArmL];
	worldTransforms_[PartId::kArmL2].translation_ = { 0,-2.0f,0 };
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 2.5f,0,0 };
	worldTransforms_[PartId::kArmR2].Initialize();
	worldTransforms_[PartId::kArmR2].parent_ = &worldTransforms_[PartId::kArmR];
	worldTransforms_[PartId::kArmR2].translation_ = { 0,-2.0f,0 };
	//下半身
	//尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0,0,0 };
	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -2.5f,-2.5f,0 };
	worldTransforms_[PartId::kLegL2].Initialize();
	worldTransforms_[PartId::kLegL2].parent_ = &worldTransforms_[PartId::kLegL];
	worldTransforms_[PartId::kLegL2].translation_ = { 0,-2.0f,0 };
	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 2.5f,-2.5f,0 };
	worldTransforms_[PartId::kLegR2].Initialize();
	worldTransforms_[PartId::kLegR2].parent_ = &worldTransforms_[PartId::kLegR];
	worldTransforms_[PartId::kLegR2].translation_ = { 0,-2.0f,0 };



}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	//視点移動処理
	{
		//Vector3 move = MathUtility::Vector3Zero();

		////視点の移動速さ
		//const float kEyeSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_W)) {
		//	move.z += kEyeSpeed;
		//}
		//else if (input_->PushKey(DIK_S)) {
		//	move.z -= kEyeSpeed;
		//}

		////視点移動（ベクトルの加算）
		//viewProjection_.eye += move;

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		////デバッグ用表示
		//debugText_->SetPos(50, 50);
		//debugText_->Printf(
		//	"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		//Vector3 move = MathUtility::Vector3Zero();

		////視点の移動速さ
		//const float kTargetSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_LEFT)) {
		//	move.x += kTargetSpeed;
		//}
		//else if (input_->PushKey(DIK_RIGHT)) {
		//	move.x -= kTargetSpeed;
		//}

		////視点移動（ベクトルの加算）
		//viewProjection_.target += move;

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		////デバッグ用表示
		//debugText_->SetPos(50, 70);
		//debugText_->Printf(
		//	"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	}

	//上方向回転処理
	{
		//	//上方向の回転速さ[ラジアン/frame]
		//	const float kUpRotSpeed = 0.05f;

		//	//押した方向で移動ベクトルを変更
		//	if (input_->PushKey(DIK_SPACE)) {
		//		viewAngle += kUpRotSpeed;
		//		//2πを超えたら0に戻す
		//		viewAngle = fmodf(viewAngle, MathUtility::PI * 2.0f);
		//	}

		//	//上方向ベクトルを計算
		//	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		//	//行列の再計算
		//	viewProjection_.UpdateMatrix();

		//	debugText_->SetPos(50, 90);
		//	debugText_->Printf(
		//		"up(%f,%f,%f", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	}

	//FoV変更処理
	{
		//const float kUpFovSpeed = 0.01f;

		////上キーで視野角が広がる
		//if (input_->PushKey(DIK_UP)) {
		//	viewProjection_.fovAngleY += kUpFovSpeed;
		//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY,MathUtility::PI);
		//}//上キーで視野角が広がる
		//else if (input_->PushKey(DIK_DOWN)) {
		//	viewProjection_.fovAngleY -= kUpFovSpeed;
		//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY,0.01f);
		//}

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 110);
		//debugText_->Printf(	"fovAngleY(Degree):%f", viewProjection_.fovAngleY * 180 / MathUtility::PI);

	}

	//クリップ距離変更処理
	{
		////上下キーでニアクリップ距離を増減
		//if (input_->PushKey(DIK_UP)) {
		//	viewProjection_.nearZ += 0.1f;
		//}else if (input_->PushKey(DIK_DOWN)) {
		//	viewProjection_.nearZ -= 0.1f;
		//}

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 130);
		//debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

	}

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
		move.x = 0.1f * (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT));

		worldTransforms_[PartId::kRoot].translation_ += move;

		//worldTransforms_[PartId::kRoot].MatUpdate();

		debugText_->SetPos(50, 130);
		debugText_->Printf(
			"rotation[kLegL]:%f,%f,%f", worldTransforms_[PartId::kLegL].rotation_.x, worldTransforms_[PartId::kLegL].rotation_.y, worldTransforms_[PartId::kLegL].rotation_.z);

		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"rotation[kLegR]:%f,%f,%f", worldTransforms_[PartId::kLegR].rotation_.x, worldTransforms_[PartId::kLegR].rotation_.y, worldTransforms_[PartId::kLegR].rotation_.z);
		debugText_->SetPos(50, 170);
		debugText_->Printf(
			"rotation[kArmL]:%f,%f,%f", worldTransforms_[PartId::kArmL].rotation_.x, worldTransforms_[PartId::kArmL].rotation_.y, worldTransforms_[PartId::kArmL].rotation_.z);
		debugText_->SetPos(50, 190);
		debugText_->Printf(
			"rotation[kArmR]:%f,%f,%f", worldTransforms_[PartId::kArmR].rotation_.x, worldTransforms_[PartId::kArmR].rotation_.y, worldTransforms_[PartId::kArmR].rotation_.z);

	}

	//上半身回転処理
	{
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[PartId::kChest].rotation_.y -= 0.05f;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[PartId::kChest].rotation_.y += 0.05f;
		}
	}

	//下半身回転処理
	{
		if (input_->PushKey(DIK_J)) {
			worldTransforms_[PartId::kHip].rotation_.y -= 0.05f;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransforms_[PartId::kHip].rotation_.y += 0.05f;
		}
	}

	//全身を回転
	if (input_->PushKey(DIK_A)) {
		worldTransforms_[PartId::kRoot].rotation_.y += 0.05f;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransforms_[PartId::kRoot].rotation_.y -= 0.05f;
	}

	if (input_->PushKey(DIK_W)) {
		//両手両足を回転
		if (!isTurn) {
			worldTransforms_[PartId::kArmL].rotation_.x -= 0.1f;
				worldTransforms_[PartId::kArmR].rotation_.x += 0.1f;
				worldTransforms_[PartId::kLegL].rotation_.x += 0.1f;
				worldTransforms_[PartId::kLegR].rotation_.x -= 0.1f;
		}
		else {
			worldTransforms_[PartId::kArmL].rotation_.x += 0.1f;
				worldTransforms_[PartId::kArmR].rotation_.x -= 0.1f;
				worldTransforms_[PartId::kLegL].rotation_.x -= 0.1f;
			worldTransforms_[PartId::kLegR].rotation_.x += 0.1f;
		}

		const float turnMax = 1.5f;


		if (worldTransforms_[PartId::kArmL].rotation_.x < -turnMax) {
			isTurn = true;
			worldTransforms_[PartId::kArmL].rotation_.x = -turnMax;
			worldTransforms_[PartId::kArmR].rotation_.x = turnMax;
			worldTransforms_[PartId::kLegL].rotation_.x = turnMax;
			worldTransforms_[PartId::kLegR].rotation_.x = -turnMax;

		}
		else if (worldTransforms_[PartId::kArmL].rotation_.x > turnMax) {
			isTurn = false;
			worldTransforms_[PartId::kArmL].rotation_.x = turnMax;
			worldTransforms_[PartId::kArmR].rotation_.x = -turnMax;
			worldTransforms_[PartId::kLegL].rotation_.x = -turnMax;
			worldTransforms_[PartId::kLegR].rotation_.x = turnMax;
		}
	}

	//子の更新
	{
		for (int i = 0; i < kNumPartId; i++) {
			worldTransforms_[i].MatUpdate();
		}
	}


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//3Dモデル描画
		//model_->Draw(worldTransforms_[0], viewProjection_, texutureHandle_);
	//	model_->Draw(worldTransforms_[1], viewProjection_, texutureHandle_);
	for (int i = 0; i < kNumPartId; i++) {
		if (i != PartId::kRoot && i != PartId::kSpine) {
			model_->Draw(worldTransforms_[i], viewProjection_, texutureHandle_);

		}
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

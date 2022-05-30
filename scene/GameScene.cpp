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


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			//ワールドトランスフォームの初期化
			worldTransforms_[i][j].Initialize();
			worldTransforms_[i][j].scale_ = {1,1,1};
			worldTransforms_[i][j].rotation_ = { 0,0,0 };
			worldTransforms_[i][j].translation_ = {i * 4.0f - 16.0f,j * 4.0f - 16.0f,0.0f };
			worldTransforms_[i][j].MatUpdate();
	
		}
	}
	


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

	////キャラクター移動処理
	//{
	//	//キャラクターの移動ベクトル
	//	Vector3 move = { 0,0,0 };
	//	move.x = 0.1f * (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT));

	//	worldTransforms_[PartId::kRoot].translation_ += move;
	//	
	//	//worldTransforms_[PartId::kRoot].MatUpdate();

	//	debugText_->SetPos(50, 130);
	//	debugText_->Printf(
	//		"pos[0]:%f,%f,%f", worldTransforms_[0].translation_.x,worldTransforms_[0].translation_.y,worldTransforms_[0].translation_.z);
	//}

	////上半身回転処理
	//{
	//	if (input_->PushKey(DIK_U)) {
	//		worldTransforms_[PartId::kChest].rotation_.y -= 0.05f;
	//	}else if (input_->PushKey(DIK_I)) {
	//		worldTransforms_[PartId::kChest].rotation_.y += 0.05f;
	//	}
	//}

	////下半身回転処理
	//{
	//	if (input_->PushKey(DIK_J)) {
	//		worldTransforms_[PartId::kHip].rotation_.y -= 0.05f;
	//	}
	//	else if (input_->PushKey(DIK_K)) {
	//		worldTransforms_[PartId::kHip].rotation_.y += 0.05f;
	//	}
	//}

	////子の更新
	//{
	//	for (int i = 0; i < kNumPartId; i++) {
	//		worldTransforms_[i].MatUpdate();
	//	}
	//}

	
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
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (i % 2 != 1|| j % 2 != 1) {
				model_->Draw(worldTransforms_[i][j], viewProjection_, texutureHandle_);
			}
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

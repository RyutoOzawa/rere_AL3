﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include"MathUtility.h"
#include<random>
#include"Enemy.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete reticle;
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
	enemyTexutureHandle_ = TextureManager::Load("orangeBlock.png");
	reticleTexture_ = TextureManager::Load("reticle.png");

	//3Dモデルの生成
	model_ = Model::Create();

	//自キャラの生成
	Player* newPlayer =new Player();
	newPlayer->Initialize(model_, texutureHandle_);
	//生成したプレイヤーをしまう
	player_.reset(newPlayer);

	//レティクルの生成
	reticle = Sprite::Create(reticleTexture_, { 300,300 });


	Vector3 enemyPos{ 10.0f,10.0f,100.0f };

	//敵の生成
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, enemyTexutureHandle_, enemyPos);
	//生成した敵をしまう
	enemy_.reset(newEnemy);

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,50,0 };

	//カメラ注視点座標を設定
	viewProjection_.target = { 0,0,1 };

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


}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (!isDebugCameraActive_)isDebugCameraActive_ = true;
		else isDebugCameraActive_ = false;
	}
#endif
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		if (input_->PushKey(DIK_J)) {
			viewProjection_.target.z += 0.05f;
		}
		else if (input_->PushKey(DIK_K)) {
			viewProjection_.target.z -= 0.05f;
		}
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}


	//自キャラの更新
	player_->Update();

	reticle->SetPosition(Vector2{ player_->GetWorldTransform().translation_.x, player_->GetWorldTransform().translation_.z + 5.0f });

	if (enemy_) {
		enemy_->Update();
	}

	



	debugText_->SetPos(50, 200);
	debugText_->Printf("cameraPos:%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 220);
	debugText_->Printf("cameraTarget:%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 240);
	debugText_->Printf("camerafov:%f", viewProjection_.fovAngleY);
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
	
		//自キャラ描画
		player_->Draw(viewProjection_);

		//敵描画
		if (enemy_) {
			enemy_->Draw(viewProjection_);
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

	reticle->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

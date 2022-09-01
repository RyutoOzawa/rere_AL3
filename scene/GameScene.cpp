#include "GameScene.h"
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
	delete modelReticle_;
	delete debugCamera_;
	delete playerModel;
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

	//座標用の乱数範囲の指定
	std::uniform_real_distribution<float> posDist(-10, 10);
	//回転角用の乱数範囲の指定
	std::uniform_real_distribution<float> rotDist(0, 360);

	//ファイル名を指定してテクスチャを読み込む
	texutureHandle_ = TextureManager::Load("player.jpg");
	enemyTexutureHandle_ = TextureManager::Load("orangeBlock.png");
	reticleTexture_ = TextureManager::Load("reticle.png");
	titleTexture_ = TextureManager::Load("title.png");
	gameoverTex_ = TextureManager::Load("gameover.png");

	title = Sprite::Create(titleTexture_, { 0, 0 });
	gameover = Sprite::Create(gameoverTex_, { 0,0 });

	//3Dモデルの生成
	model_ = Model::Create();

	//自キャラの生成
	playerModel = Model::CreateFromOBJ("player3D", true);
	Player* newPlayer =new Player();
	newPlayer->Initialize(playerModel, texutureHandle_);
	//生成したプレイヤーをしまう
	player_.reset(newPlayer);

	//レティクルの生成
	modelReticle_ = Model::CreateFromOBJ("redReticle", true);
	player_->reticle.Initialize(modelReticle_);

	Vector3 enemyPos{ posDist(engine),0.0f,100.0f};

	//敵の生成
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, enemyTexutureHandle_, enemyPos);
	//生成した敵をしまう
	enemy_.reset(newEnemy);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());

	for (int i = 0; i < 10; i++) {
		enemys[i].Initialize(model_, enemyTexutureHandle_, enemyPos);

		enemys[i].SetPlayer(player_.get());
	}

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

	////軸方向表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	spawnTime = spawnCooltime;
}

void GameScene::Update() {
	if (scene == 0) {
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 1;
		}
	}
	else if (scene == 1) {
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());


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


		if (enemy_) {
			enemy_->Update();
		}

		for (int i = 0; i < 10; i++) {
			if (enemys[i].GetAlive()) {
				enemys[i].Update();
			}
		}

		CheckAllCollisions();

		if (spawnTime > 0) {
			spawnTime--;
		}
		else if (spawnTime == 0) {
			for (int i = 0; i < 10; i++) {
				if (!enemys[i].GetAlive()) {
					std::uniform_real_distribution<float> posDist(-10, 10);
					Vector3 pos = { posDist(engine),0.0f,100.0f };
					enemys[i].Spawn({ posDist(engine), 0.0f, 100.0f });
					spawnTime = spawnCooltime;
					break;
				}
			}
		}

		//debugText_->SetPos(50, 200);
		//debugText_->Printf("cameraPos:%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

		//debugText_->SetPos(50, 220);
		//debugText_->Printf("cameraTarget:%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

		//debugText_->SetPos(50, 240);
		//debugText_->Printf("camerafov:%f", viewProjection_.fovAngleY);
	}
	else if (scene == 2) {
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 0;
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
	/// 
	if (scene ==0) {
		title->Draw();
	}
	else if (scene == 2) {
		gameover->Draw();
	}

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
	if (scene == 1) {
		//自キャラ描画
		player_->Draw(viewProjection_);

		//敵描画
		if (enemy_) {
			enemy_->Draw(viewProjection_);
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

	//reticle->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;
	
	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();



#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPos();

	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
		posB = enemyBullet->GetWorldPos();

		float len = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

		if (len <= 1.0f && !enemyBullet->IsDead()) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();

			//敵弾の衝突時コールバックを呼び出す
			enemyBullet->OnCollision();
			scene = 2;
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetworldPos();

	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		posB = playerBullet->GetWorldPos();

		float len = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

		if (len <= 1.0f) {
			//自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();

			//敵弾の衝突時コールバックを呼び出す
			playerBullet->OnCollision();
		
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
#pragma endregion
}

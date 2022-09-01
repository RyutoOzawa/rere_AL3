#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t texutureHandle_ = 0;
	uint32_t enemyTexutureHandle_ = 0;
	uint32_t reticleTexture_ = 0;
	uint32_t titleTexture_ = 0;
	uint32_t gameoverTex_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* playerModel = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//レティクル
	Model* modelReticle_ = nullptr;

	int spawnCooltime = 60 * 2;

	int spawnTime = 0;

	Enemy enemys[10];

	Sprite* title = nullptr;
	Sprite* gameover = nullptr;

	int scene = 0;
public:

	//自キャラ
	std::unique_ptr<Player> player_;

	std::unique_ptr<Enemy> enemy_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

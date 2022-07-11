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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t texutureHandle_ = 0;
	uint32_t enemyTexutureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	WorldTransform obj;

public:

	//自キャラ
	std::unique_ptr<Player> player_;

	std::unique_ptr<Enemy> enemy_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	/// <summary>
	/// レイの判定
	/// </summary>
	/// <param name="startRay">レイの始点</param>
	/// <param name="endRay">レイの終点</param>
	/// <param name="obj">オブジェクト</param>
	/// <returns></returns>
	bool CollisionRayToObj(Vector3 startRay, Vector3 endRay, WorldTransform obj);

};

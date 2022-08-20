#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"input.h"
#include"DebugText.h"
#include"EnemyBullet.h"
#include<memory>
#include<list>

//行動フェーズ
enum class Phase {
	Approach,	//接近する
	Leave,		//離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 接近フェーズの初期化処理
	/// </summary>
	void InitializeApproach();


	/// <summary>
	/// 接近フェーズの更新処理
	/// </summary>
	void UpdateApproach();

	/// <summary>
	/// 離脱フェーズの更新処理
	/// </summary>
	void UpdateLeave();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//弾の発射タイマー
	int32_t fireTimer = 0;

public:
	//発射間隔
	static const int kFireInterval = 60;
};


#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"input.h"
#include"DebugText.h"
#include"EnemyBullet.h"
#include<memory>
#include<list>

//自機クラスの前方宣言
class Player;


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

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetworldPos();

	//衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets_; }

	bool GetAlive() { return isAlive; }

	void Spawn(Vector3 pos);
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

	//自キャラ
	Player* player_ = nullptr;

	//弾の発射タイマー
	int32_t fireTimer = 0;

	bool isAlive = false;

	int spawnInterval = 60;
	int spawnTimer = 0;

public:
	//発射間隔
	static const int kFireInterval = 180;
};


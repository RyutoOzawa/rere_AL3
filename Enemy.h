#pragma once
#include"Model.h"
#include"WorldTransform.h"

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
};


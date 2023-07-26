#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "input/Input.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player {
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model_body, Model* model_head, Model* model_l_arm, Model* model_r_arm);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform();

	void SetViewProjection(const ViewProjection* viewProjection);

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	// 腕ふりギミック初期化
	void InitializeArmGimmick();

	// 腕ふりギミック更新
	void UpdateArmGimmick();

private: // メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransform_body_;
	WorldTransform worldTransform_head_;
	WorldTransform worldTransform_l_arm_;
	WorldTransform worldTransform_r_arm_;


	// モデル
	Model* model_body_ = nullptr;
	Model* model_head_ = nullptr;
	Model* model_l_arm_ = nullptr;
	Model* model_r_arm_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<flame>
	float floatingPeriod_ = 60.0f; 
	// 浮遊の振幅<m>
	float floatingAmplitude = 60.0f;

	// 浮遊ギミックの媒介変数
	float armParameter_ = 0.0f;
	// 浮遊移動のサイクル<flame>
	float armPeriod_ = 60.0f;
	// 浮遊の振幅<m>
	float armAmplitude = 60.0f;
};
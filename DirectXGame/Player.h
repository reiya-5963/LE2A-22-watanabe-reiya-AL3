#pragma once
#include "BaseCharacter.h"

#include "input/Input.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
	// モデルの配列番号
	enum modelIndex{
		ModelIndexBody = 0,
		ModelIndexHead,
		ModelIndexL_Arm,
		ModelIndexR_Arm,
	};

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 
	/// </summary>
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
	//WorldTransform worldTransform_;
	WorldTransform worldTransform_body_;
	WorldTransform worldTransform_head_;
	WorldTransform worldTransform_l_arm_;
	WorldTransform worldTransform_r_arm_;

	// キーボード入力
	Input* input_ = nullptr;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<flame>
	float floatingPeriod_ = 60.0f;
	// 浮遊の振幅<m>
	float floatingAmplitude = 60.0f;

	// 腕ふりギミックの媒介変数
	float armParameter_ = 0.0f;
	// 腕ふりのサイクル<flame>
	float armPeriod_ = 60.0f;
	// 腕ふりの振幅<m>
	float armAmplitude = 60.0f;
};
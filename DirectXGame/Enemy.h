#pragma once

#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
	enum modelIndex {
		ModelIndexBody = 0,
		ModelIndexFireWepon,
		ModelIndexIceWeapon,
	};
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデル</param>
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


private: // メンバ変数
	//
	WorldTransform worldTransform_body_;
	WorldTransform worldTransform_f_weapon_;
	WorldTransform worldTransform_i_weapon_;
};
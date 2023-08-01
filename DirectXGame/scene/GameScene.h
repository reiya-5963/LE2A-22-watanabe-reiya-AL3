#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


#include <memory>
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "Ground.h"
#include "FollowCamera.h"

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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;


	// ビュープロジェクション
	ViewProjection viewProjection_;



	// モデル
	std::unique_ptr<Model> P_model_body = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_head = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_l_arm = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_r_arm = nullptr;
	//
	std::unique_ptr<Model> P_model_wepon = nullptr;



	// モデル
	std::unique_ptr<Model> E_model_body = nullptr;
	// モデル
	std::unique_ptr<Model> E_model_I_Wepon = nullptr;
	// モデル
	std::unique_ptr<Model> E_model_F_Wepon = nullptr;



	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;



	// モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_= nullptr;

	// モデル
	std::unique_ptr<Model> groundModel_ = nullptr;
	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;
	
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
};

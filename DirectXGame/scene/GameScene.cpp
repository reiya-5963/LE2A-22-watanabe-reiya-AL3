#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//
	//
	//// モデルの開放
	// delete model_;

	//// プレイヤーの開放
	// delete player_;
}

void GameScene::Initialize() {

#pragma region DirectXと入力系と音の初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
#pragma endregion

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// モデルの生成
	P_model_body.reset(Model::CreateFromOBJ("TestPlayerver_body", true));
	P_model_head.reset(Model::CreateFromOBJ("TestPlayerver_head", true));
	P_model_l_arm.reset(Model::CreateFromOBJ("TestPlayerver_l_arm", true));
	P_model_r_arm.reset(Model::CreateFromOBJ("TestPlayerver_r_arm", true));
	P_model_wepon.reset(Model::CreateFromOBJ("wepon", true));

	E_model_body.reset(Model::CreateFromOBJ("EnemyTest_Body", true));
	E_model_F_Wepon.reset(Model::CreateFromOBJ("EnemyTest_F_Wepon", true));
	E_model_I_Wepon.reset(Model::CreateFromOBJ("EnemyTest_I_Wepon", true));


	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(skydomeModel_.get(), {0, 0, 0});

	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化
	ground_->Initialize(groundModel_.get(), {0, 0, 0});

	// プレイヤーの生成
	player_ = std::make_unique<Player>();

	// 敵の生成
	enemy_ = std::make_unique<Enemy>();


	// プレイヤーの初期化
	std::vector<Model*> playerModels = {
	    P_model_body.get(), 
		P_model_head.get(), 
		P_model_l_arm.get(),
		P_model_r_arm.get(),
	    P_model_wepon.get()};

		// 敵の初期化
	std::vector<Model*> enemyModels = {
	    E_model_body.get(), 
		E_model_F_Wepon.get(), 
		E_model_I_Wepon.get()};



	// プレイヤーの初期化
	player_->Initialize(playerModels);

	// 敵の初期化
	enemy_->Initialize(enemyModels);
	enemy_->SetVelocity({0, 0, 1});
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		}
		else if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
	}
	
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif
	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// 追従カメラの更新
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// プレイヤーの更新
	player_->Update();

	// プレイヤーの更新
	//enemy_->SetVelocity({0, 0, 1});
	enemy_->Update();

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.TransferMatrix();
		//viewProjection_.UpdateMatrix();
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

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 地面の描画
	ground_->Draw(viewProjection_);



	// プレイヤーの描画
	player_->Draw(viewProjection_);

	// プレイヤーの描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

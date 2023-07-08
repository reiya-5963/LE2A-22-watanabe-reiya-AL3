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

	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	// モデルの生成
	model_.reset(Model::Create());
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
	// プレイヤーの初期化
	player_->Initialize(model_.get(), textureHandle_);
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
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
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
	}



	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();


	// プレイヤーの更新
	player_->Update();
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

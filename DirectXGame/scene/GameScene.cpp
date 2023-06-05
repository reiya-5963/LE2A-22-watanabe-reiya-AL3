#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete debugCamera_;
	delete skydome_;
	delete railCamera_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	//	viewProjection_.farZ = 50.0f;
	viewProjection_.Initialize();

	//
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 自キャラの生成
	player_ = new Player();

	/*Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, {0, 2, 10}, this);
	newEnemy->SetPlayer(player_);*/

	//enemys_.push_back(newEnemy);

	// enemy_ = new Enemy();

	skydome_ = new Skydome();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//// 自キャラの初期化
	// enemy_->Initialize(model_, {0, 2, 10}, this);
	// enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_, {0, 0, 0});

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -100.0f}, player_->GetWorldMatrix().rotation_);

	player_->SetParent(&railCamera_->GetWorldMatrix());
	LoadEnemyPopData();
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_W)) {
		isDebugCameraActive_ = true;
	}
#endif
	ImGui::Begin("Debug");
	ImGui::Text("isDebugCameraActive : %d", isDebugCameraActive_);
	ImGui::End();

	// 自キャラの更新
	player_->Update();
	UpdateEnemyPopCommands();
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	// enemy_->Update();
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	skydome_->Update();

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		// viewProjection_.UpdateMatrix();
	}

	CheckAllCollisions();
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
	// 自キャラの描画
	player_->Draw(viewProjection_);
	// enemy_->Draw(viewProjection_);
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);

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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }
void GameScene::AddEnemy(Enemy* enemy) { enemys_.push_back(enemy); }

void GameScene::EnemyIni(Model* model, const Vector3 position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model, position, this);
	newEnemy->SetPlayer(player_);
	AddEnemy(newEnemy);
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();

		float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float playerRad = 2.5f;
		float enemyRad = 2.5f;
		if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラ

	for (PlayerBullet* plaBullet : playerBullets) {
		posB = plaBullet->GetWorldPosition();
		for (EnemyBullet* eneBullet : enemyBullets_) {
			posA = eneBullet->GetWorldPosition();

			float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float playerRad = 2.5f;
			float enemyRad = 2.5f;
			if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
				plaBullet->OnCollision();
				eneBullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾

	for (Enemy* enemy : enemys_) {
		posB = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posA = bullet->GetWorldPosition();

			float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float playerRad = 2.5f;
			float enemyRad = 2.5f;
			if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open("./resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	if (isWait_) {
		waitTimer_--;

		if (waitTimer_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		} 
		else if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyIni(model_, Vector3(x, y, z));
		} 
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;
			break;
		}

		
	}
}

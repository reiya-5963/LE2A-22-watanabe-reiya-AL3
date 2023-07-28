#include "Player.h"
#include "MyMath.h"
#include <cassert>
#include "ImGuiManager.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(const std::vector<Model*>& models) {
	// NULLチェック
	assert(models[ModelIndexR_Arm]);

	// インプット系の初期化
	input_ = Input::GetInstance();

	// ベース部分の初期化
	BaseCharacter::Initialize(models);

	// 各部位のワールドトランスフォーム初期化
	worldTransform_body_.Initialize();
	worldTransform_head_.Initialize();
	worldTransform_l_arm_.Initialize();
	worldTransform_r_arm_.Initialize();

	// 各部位の位置初期化 //
	worldTransform_body_.parent_ = &worldTrans_;
	worldTransform_head_.parent_ = &worldTransform_body_;
	worldTransform_head_.translation_.y += 4.3f;

	worldTransform_l_arm_.parent_ = &worldTransform_body_;
	worldTransform_l_arm_.translation_.y += 4.0f;
	worldTransform_l_arm_.translation_.x -= 1.0f;

	worldTransform_r_arm_.parent_ = &worldTransform_body_;
	worldTransform_r_arm_.translation_.y += 4.0f;
	worldTransform_r_arm_.translation_.x += 1.0f;
	//****************//

	// 各ギミックの初期化
	InitializeFloatingGimmick();
	InitializeArmGimmick();
	worldTrans_.rotation_.y = std::atan2(worldTrans_.rotation_.x, worldTrans_.rotation_.z);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	Matrix4x4 movetrans = MyMath::MakeTranslateMatrix(worldTrans_.translation_);
	Vector3 move = {0, 0, 0};
	XINPUT_STATE joyState;
	// もしコントローラーでのプレイなら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};
		//
		move = MyMath::Normalize(move);
		move.x *= speed;
		move.y *= speed;
		move.z *= speed;

		Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(move);
		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_->rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_->rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_->rotation_.z));

		moveMat = MyMath::Multiply(moveMat, rotateMat);
		move.x = moveMat.m[3][0];
		move.y = moveMat.m[3][1];
		move.z = moveMat.m[3][2];

		worldTrans_.rotation_.y = std::atan2(move.x, move.z);

	}
	// そうでないならキーマウ
	else {

		// 速さ
		const float speed = 0.3f;

		if (input_->PushKey(DIK_W)) {
			move.z = 1.0f;
		}
		if (input_->PushKey(DIK_S)) {
			move.z = -1.0f;
		}
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		move = MyMath::Normalize(move);
		move.x *= speed;
		move.y *= speed;
		move.z *= speed;

		Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(move);
		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_->rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_->rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_->rotation_.z));

		moveMat = MyMath::Multiply(moveMat, rotateMat);
		move.x = moveMat.m[3][0];
		move.y = moveMat.m[3][1];
		move.z = moveMat.m[3][2];

		worldTrans_.rotation_.y = std::atan2(move.x, move.z);
	}

	// 位置の移動
	worldTrans_.translation_ = MyMath::TransformCoord(move, movetrans);
	
	// 各ギミックの更新処理
	UpdateFloatingGimmick();
	UpdateArmGimmick();

	
	ImGui::Begin("Player");
	ImGui::SliderFloat3("head", &worldTransform_head_.translation_.x, 0.0f, 50.0f);
	ImGui::SliderFloat3("l_arm", &worldTransform_l_arm_.translation_.x, 0.0f, -50.0f);
	ImGui::SliderFloat3("r_arm", &worldTransform_r_arm_.translation_.x, 0.0f, 50.0f);
	ImGui::SliderFloat("period", &floatingPeriod_, 0.0f, 500.0f);
	ImGui::SliderFloat("amplitude", &floatingAmplitude, 0.0f, 500.0f);
	ImGui::SliderFloat("armperiod", &armPeriod_, 0.0f, 500.0f);
	ImGui::SliderFloat("armamplitude", &armAmplitude, 0.0f, 500.0f);
	ImGui::End();

	// ベース部分の更新処理
	BaseCharacter::Update();

	// 各部位のワールドトランスフォームの更新処理
	worldTransform_body_.UpdateMatrix();
	worldTransform_head_.UpdateMatrix();
	worldTransform_l_arm_.UpdateMatrix();
	worldTransform_r_arm_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	models_[ModelIndexBody]->Draw(worldTransform_body_, viewProjection);
	models_[ModelIndexHead]->Draw(worldTransform_head_, viewProjection);
	models_[ModelIndexL_Arm]->Draw(worldTransform_l_arm_, viewProjection);
	models_[ModelIndexR_Arm]->Draw(worldTransform_r_arm_, viewProjection);
}

void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;

	floatingPeriod_ = 60.0f;

	floatingAmplitude = 0.5f;
}

void Player::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / floatingPeriod_;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを越えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	// 浮遊を座標に反映
	worldTransform_body_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;


}

void Player::InitializeArmGimmick() { 
	armParameter_ = 0.0f; 
	armPeriod_ = 40.0f;
	armAmplitude = 0.15f;
}

void Player::UpdateArmGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / armPeriod_;

	// パラメータを1ステップ分加算
	armParameter_ += step;
	// 2πを越えたら0に戻す
	armParameter_ = std::fmod(armParameter_, 2.0f * 3.14f);

	// 浮遊を座標に反映
	worldTransform_l_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
	worldTransform_r_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
}

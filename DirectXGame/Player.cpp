#include "Player.h"
#include "MyMath.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model) {
	// NULLチェック
	assert(model);
	
	model_ = model;

	input_ = Input::GetInstance();


	worldTransform_.Initialize();

}

/// <summary>
/// 更新
/// </summary>
void Player::Update() { 
	Matrix4x4 movetrans = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	Vector3 move = {0, 0, 0};
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		move = {
			(float)joyState.Gamepad.sThumbLX,
			0.0f, 
			(float)joyState.Gamepad.sThumbLY};
		// 
		move = MyMath::Normalize(move);
		move.x *= speed;
		move.y *= speed;
		move.z *= speed;


	} else {
	
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


	}
	
	worldTransform_.translation_ = MyMath::TransformCoord(move, movetrans);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

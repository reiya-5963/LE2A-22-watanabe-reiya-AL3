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

	worldTransform_.Initialize();

}

/// <summary>
/// 更新
/// </summary>
void Player::Update() { 
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbRX, 0.0f, (float)joyState.Gamepad.sThumbRY};
		// 
		move = MyMath::Normalize(move);
		move.x* speed;
		move.y* speed;
		move.z* speed;

		// 
		worldTransform_.translation_ += ;



	}
	
	
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}
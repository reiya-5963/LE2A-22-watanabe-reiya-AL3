#include <Player.h>
#include <cassert>
#include "ImGuiManager.h"

#include "MyMath.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();


}


void Player::Update() { 

	
	Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 rotateMatrixX = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotateMatrixXYZ = Multiply(Multiply(rotateMatrixX, rotateMatrixY), rotateMatrixZ);

	Matrix4x4 translateMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	
		Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	

	worldTransform_.translation_ = TransformCoord(move, translateMatrix);

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	float pos[3]{};
	pos[0] = worldTransform_.translation_.x;
	pos[1] = worldTransform_.translation_.y;
	pos[2] = worldTransform_.translation_.z;

	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Player", pos, -500.0f, 500.0f);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];

	ImGui::End();








	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	
	
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);



}
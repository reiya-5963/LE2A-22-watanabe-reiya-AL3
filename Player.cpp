#include "Player.h"



void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();


}


void Player::Update() { 

	
	Matrix4x4 scaleMatrix = MyMath::MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 rotateMatrixX = MyMath::MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotateMatrixY = MyMath::MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotateMatrixZ = MyMath::MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotateMatrixXYZ =
	    MyMath::Multiply(MyMath::Multiply(rotateMatrixX, rotateMatrixY), rotateMatrixZ);

	Matrix4x4 translateMatrix = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	
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

	Rotate();

	Attack();

	if (bullet_) {
		bullet_->Update();
	}


	worldTransform_.translation_ = MyMath::TransformCoord(move, translateMatrix);

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

	worldTransform_.UpdateMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_) {
		bullet_->Draw(viewProjection);
	}

}


void Player::Rotate() {
	//
	const float kRotSpeed = 0.02f;

	//
	if (input_->PushKey(DIK_A)) {
		MyMath::MakeRotateYMatrix(worldTransform_.rotation_.y -= kRotSpeed);
	} else if (input_->PushKey(DIK_D)) {
		MyMath::MakeRotateYMatrix(worldTransform_.rotation_.y += kRotSpeed);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
	
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet;
	}
}
#include "Player.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();


}


void Player::Update() { 

	
	Matrix4x4 scaleMatrix = MyMath::MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 translateMatrix = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

		Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	Rotate();

	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}


	worldTransform_.translation_ = MyMath::TransformCoord(move, translateMatrix);

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	/*float pos[3]{};
	pos[0] = worldTransform_.translation_.x;
	pos[1] = worldTransform_.translation_.y;
	pos[2] = worldTransform_.translation_.z;

	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Player", pos, -500.0f, 500.0f);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];

	ImGui::End();*/

	worldTransform_.UpdateMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}


void Player::Rotate() {
	//
	const float kRotSpeed = 0.02f;

	//
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		MyMath::MakeRotateYMatrix(worldTransform_.rotation_.y += kRotSpeed);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = MyMath::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		bullets_.push_back(newBullet);
	}
}
#include "Player.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	worldTransform_.translation_.z = 50.0f;

	
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

	worldTransform_.translation_ = MyMath::TransformCoord(move, translateMatrix);

	Rotate();

	Attack();

	worldTransform_.UpdateMatrix(); 


	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}



	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -500.0f, 500.0f);
	ImGui::End();

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
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3     velocity(0, 0, kBulletSpeed);

		velocity = MyMath::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	/*worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;*/
	return worldPos;
}

void Player::OnCollision() {

}
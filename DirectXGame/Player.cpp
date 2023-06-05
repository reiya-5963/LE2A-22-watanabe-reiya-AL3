#include "Player.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	worldTransform_.translation_.z = 50.0f;

	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	worldTransform3DReticle_.Initialize();


	sprite2DReticle_ = Sprite::Create(
	    texReticle_,
	    {
	        WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2
	    },
	    {1, 1, 1, 1}, {0.5f, 0.5f});
	
}


void Player::Update(ViewProjection& viewProjection) { 

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





		const float kDistancePlayerTo3DReticle = 50.0f;
	//Matrix4x4 Reticle3DTransMat =
	//    MyMath::MakeTranslateMatrix(worldTransform3DReticle_.translation_);

	Vector3 offSet = {0, 0, 1.0f};
	//Matrix4x4 worMat = MyMath::MakeAffineMatrix(
	//    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_); 

	offSet = MyMath::TransformNormal(offSet, worldTransform_.matWorld_);
	offSet = MyMath::FVMultiply(kDistancePlayerTo3DReticle, MyMath::Normalize(offSet));
	worldTransform3DReticle_.translation_.x = offSet.x + worldTransform_.matWorld_.m[3][0];
	worldTransform3DReticle_.translation_.y = offSet.y + worldTransform_.matWorld_.m[3][1];
	worldTransform3DReticle_.translation_.z = offSet.z + worldTransform_.matWorld_.m[3][2];

	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];


	Matrix4x4 matViewPort =
	    MyMath::MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewPort = MyMath::MMMultiply(
	    viewProjection.matView, MyMath::MMMultiply(viewProjection.matProjection, matViewPort));

	positionReticle = MyMath::TransformCoord(positionReticle, matViewProjectionViewPort);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));



	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;


	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -500.0f, 500.0f);
	ImGui::End();


	



	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}



}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
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
	if (input_->PushKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		//Vector3     velocity(0, 0, kBulletSpeed);
		Vector3 velocity;
		Vector3 Reticle3DPos;
		Reticle3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
		Reticle3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
		Reticle3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

		velocity = Reticle3DPos - GetWorldPosition();
		velocity = MyMath::FVMultiply(kBulletSpeed, MyMath::Normalize(velocity));
		/*MyMath::TransformNormal(velocity, worldTransform_.matWorld_);*/

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

	/*if (worldTransform_.parent_) {
		
	}*/
	return worldPos;
}

void Player::OnCollision() {

}
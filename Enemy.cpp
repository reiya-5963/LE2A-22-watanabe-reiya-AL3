#include "Enemy.h"

void Enemy::Initialize(
    Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");
	;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void Enemy::Update() { 

	switch (phase_) {
	case Enemy::Phase::Approach:
		Vector3 appVelocity(0, 0, -0.25f);
		worldTransform_.translation_ = MyMath::VecAdd(worldTransform_.translation_, appVelocity);

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		break;
	case Enemy::Phase::Leave:
		Vector3 leaveVelocity(-0.25f, 0.25f, 0);
		worldTransform_.translation_ = MyMath::VecAdd(worldTransform_.translation_, leaveVelocity);

		break;
	}

	worldTransform_.UpdateMatrix(); 
}


void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

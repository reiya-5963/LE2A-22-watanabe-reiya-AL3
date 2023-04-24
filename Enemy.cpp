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
	const float kBulletSpeed = 0.5f;
	Vector3 velocity(0, 0, -kBulletSpeed);
	worldTransform_.translation_ = MyMath::VecAdd(worldTransform_.translation_, velocity);

	worldTransform_.UpdateMatrix(); 
}


void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

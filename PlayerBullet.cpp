#include "PlayerBullet.h"
#include <cassert>
#include "MyMath.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
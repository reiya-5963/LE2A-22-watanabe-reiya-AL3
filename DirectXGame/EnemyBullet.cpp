#include "EnemyBullet.h"
#include "MyMath.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() {
	isDead_ = true; 
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos = MyMath::TransformCoord(worldTransform_.translation_, worldTransform_.matWorld_);

	/*worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;*/

	return worldPos;
}

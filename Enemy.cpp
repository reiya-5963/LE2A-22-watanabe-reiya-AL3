#include "Enemy.h"

void Enemy::Initialize(
    Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");
	;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	pApproachMove = &Enemy::ApproachMove;

	

}

void Enemy::Update() { 

	/*switch (phase_) {
	case Enemy::Phase::Approach:
		ApproachMove();

		break;
	case Enemy::Phase::Leave:
		LeaveMove();
		break;
	}*/
	(this->*spMoveTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix(); 
}


void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachMove() {
	Vector3 appVelocity(0, 0, -0.25f);
	worldTransform_.translation_ = MyMath::VecAdd(worldTransform_.translation_, appVelocity);

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveMove() {
	Vector3 leaveVelocity(-0.25f, 0.25f, 0);
	worldTransform_.translation_ = MyMath::VecAdd(worldTransform_.translation_, leaveVelocity);
}

void (Enemy::*Enemy::spMoveTable[])() = {
	&Enemy::ApproachMove,
	&Enemy::LeaveMove
};
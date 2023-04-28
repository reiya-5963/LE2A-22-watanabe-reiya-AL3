#include "Enemy.h"

void Enemy::Initialize(
    Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");
	;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	
	state = new EnemyStateApproah();
	state->SetEnemy(this);

	/*pApproachMove = &Enemy::ApproachMove;
	phase_ = Phase::Approach;*/
	
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

	//(this->*spMoveTable[static_cast<size_t>(phase_)])();

	state->Update();

	worldTransform_.UpdateMatrix(); 
}


void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//void (Enemy::*Enemy::spMoveTable[])() = {
//	&Enemy::ApproachMove,
//	&Enemy::LeaveMove
//};

void Enemy::ChangeState(BaseEnemyState* newEnemyState) {
	delete state;
	state = newEnemyState;
	state->SetEnemy(this);
}

void Enemy::SetPosition(Vector3 velosity) {
	worldTransform_.translation_ = worldTransform_.translation_ + velosity;
}


////****EnemyState****////


void EnemyStateApproah::Update() {
	Vector3 appVelocity(0, 0, -0.25f);
	enemy_->SetPosition(appVelocity);
	if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update() {
	Vector3 leaveVelocity(-0.25f, 0.25f, 0);
	enemy_->SetPosition(leaveVelocity);
}



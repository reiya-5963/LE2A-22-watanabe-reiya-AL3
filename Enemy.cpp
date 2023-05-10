#include "Enemy.h"

Enemy::~Enemy() {
	delete state;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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
	ApproachInit();
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

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	worldTransform_.UpdateMatrix(); 
}


void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
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

void Enemy::Fire() {
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = MyMath::TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);
}

void Enemy::ApproachInit() {
	fireTimer = kFireInterval;

}


void Enemy::Reset() {
	Fire();
	
	//
	timedCells_.push_back(
		new TimedCell(std::bind(&Reset(), this), kFireInterval));

}

////****EnemyState****////




void EnemyStateApproah::Update() {
	int32_t timer_ = enemy_->fireTimer;
	enemy_->fireTimer--;

	if (timer_ == 0) {
		enemy_->Fire();

		enemy_->fireTimer = enemy_->kFireInterval;
	}

	Vector3 appVelocity(0, 0, -0.05f);
	enemy_->SetPosition(appVelocity);
	/*if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}*/
}

void EnemyStateLeave::Update() {
	Vector3 leaveVelocity(-0.25f, 0.25f, 0);
	enemy_->SetPosition(leaveVelocity);
}



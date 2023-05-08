#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "MyMath.h"
#include "EnemyBullet.h"
#include <cassert>
class Enemy;



class BaseEnemyState {
protected:
	Enemy* enemy_ = nullptr;

public:
	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	virtual void Update(){};
};



class EnemyStateApproah : public BaseEnemyState{
public:
	void Update();
};



class EnemyStateLeave : public BaseEnemyState {
public:
	void Update();
};



/// <summary>
/// enemy
/// </summary>
class Enemy {
	enum class Phase {
		Approach,
		Leave,
	};

public:
	~Enemy() { delete state; }

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ChangeState(BaseEnemyState* newEnemyState);

	WorldTransform GetWT() {return worldTransform_;}

	void SetPosition(Vector3 velosity);

	void Fire();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	BaseEnemyState* state;

	EnemyBullet* bullet_ = nullptr;

	/*Phase phase_;
	void (Enemy::*pApproachMove)();
	static void (Enemy::*spMoveTable[])();*/
};




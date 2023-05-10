#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "EnemyBullet.h"
#include <cassert>
#include <list>
#include "TimedCell.h"
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
	~Enemy();

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ChangeState(BaseEnemyState* newEnemyState);

	WorldTransform GetWT() { return worldTransform_; }

	void SetPosition(Vector3 velosity);

	void Fire();

	void ApproachInit();

	/*int32_t GetTimer() {
	    return fireTimer;
	}

	void SetTimer(int32_t timer) {
	    fireTimer = timer;
	}*/

	static const int kFireInterval = 60;
	int32_t fireTimer = 0;

	void Reset();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	BaseEnemyState* state;

	EnemyBullet* bullet_ = nullptr;

	std::list<EnemyBullet*> bullets_;

	std::list < TimedCell*> timedCells_;
	/*Phase phase_;
	void (Enemy::*pApproachMove)();
	static void (Enemy::*spMoveTable[])();*/
};







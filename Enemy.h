#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "MyMath.h"
#include <cassert>



/// <summary>
/// enemy
/// </summary>
class Enemy {
	enum class Phase {
		Approach,
		Leave,
	};

public:
	

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ApproachMove();
	void LeaveMove();


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;


	Phase phase_ = Phase::Approach;

	void (Enemy::*pApproachMove)();

	static void (Enemy::*spMoveTable[])();

	
};
#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "EnemyBullet.h"
#include <cassert>
#include <list>
class GameScene;
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

class Player;

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

	void Initialize(Model* model, const Vector3& position, GameScene* gamescene);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ChangeState(BaseEnemyState* newEnemyState);

	WorldTransform GetWT() {return worldTransform_;}

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

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	void OnCollision();
	bool IsDead() const { return isDead_; }


	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	BaseEnemyState* state;

	EnemyBullet* bullet_ = nullptr;
	GameScene* gameScene_ = nullptr;

	Player* player_ = nullptr;
	bool isDead_ = false;

	/*Phase phase_;
	void (Enemy::*pApproachMove)();
	static void (Enemy::*spMoveTable[])();*/
};







#pragma once

#include "Model.h"
#include "WorldTransform.h"


class PlayerBullet {
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	
	Vector3 velocity_{};

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};
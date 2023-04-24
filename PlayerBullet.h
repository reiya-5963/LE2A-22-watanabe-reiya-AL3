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

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	
	Vector3 velocity_;
};
#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "ImGuiManager.h"


class RailCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotate);
	void Update();
	//void Draw();

	void SetPosition(Vector3 position) {
		worldTransform_.translation_ = worldTransform_.translation_ + position;
	}

	void SetRotate(Vector3 rotate) {
		worldTransform_.rotation_ = worldTransform_.rotation_ + rotate;
	}
	void SetScale(Vector3 scale) {
		worldTransform_.scale_ = worldTransform_.scale_ + scale;
	}

	const WorldTransform& GetWorldMatrix() { 
		return worldTransform_;
	}

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

};
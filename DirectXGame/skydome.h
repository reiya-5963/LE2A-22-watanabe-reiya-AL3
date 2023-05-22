#pragma once
#include "Model.h"

#include "WorldTransform.h"

class skydome {
public:
	void Initialize();

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};
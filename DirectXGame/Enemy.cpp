#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(const std::vector<Model*>& models) {
	//
	assert(models[ModelIndexIceWeapon]);

	//
	BaseCharacter::Initialize(models);

	//
	worldTransform_body_.Initialize();
	worldTransform_f_weapon_.Initialize();
	worldTransform_i_weapon_.Initialize();

	worldTransform_body_.parent_ = &worldTrans_;
	worldTransform_f_weapon_.parent_ = &worldTransform_body_;
	worldTransform_f_weapon_.translation_.y += 1.5f;
	worldTransform_f_weapon_.translation_.x -= 1.5f;
	worldTransform_i_weapon_.parent_ = &worldTransform_body_;
	worldTransform_i_weapon_.translation_.y += 1.5f;
	worldTransform_i_weapon_.translation_.x += 1.5f;
}

void Enemy::Update() { 
	BaseCharacter::Update();
	worldTransform_body_.UpdateMatrix();
	worldTransform_f_weapon_.UpdateMatrix();
	worldTransform_i_weapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[ModelIndexBody]->Draw(worldTransform_body_, viewProjection);
	models_[ModelIndexFireWepon]->Draw(worldTransform_f_weapon_, viewProjection);
	models_[ModelIndexIceWeapon]->Draw(worldTransform_i_weapon_, viewProjection);


}
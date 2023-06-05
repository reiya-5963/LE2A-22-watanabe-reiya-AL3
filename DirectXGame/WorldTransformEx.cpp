#include "WorldTransform.h"
#include "MyMath.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_); 

	if (parent_) {
		matWorld_ = MyMath::MMMultiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}

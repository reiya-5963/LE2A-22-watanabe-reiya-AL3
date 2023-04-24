#include "WorldTransform.h"
#include "MyMath.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_); 

	TransferMatrix();
}

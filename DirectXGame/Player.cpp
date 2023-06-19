#include "Player.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	worldTransform_.translation_.z = 50.0f;

	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	worldTransform3DReticle_.Initialize();
	//worldTransform3DReticle_.parent_ = &worldTransform_;

	sprite2DReticle_ = Sprite::Create(
	    texReticle_,
	    {
	        WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2
	    },
	    {1, 1, 1, 1}, {0.5f, 0.5f});
	
}


void Player::Update(ViewProjection& viewProjection) { 
	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	
	Matrix4x4 scaleMatrix = MyMath::MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 translateMatrix = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	//
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}



	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ = MyMath::TransformCoord(move, translateMatrix);

	Rotate();

	Attack();

	
	worldTransform_.UpdateMatrix(); 




	

	MouseUpdate(viewProjection);

	//const float kDistancePlayerTo3DReticle = 50.0f;
	//Matrix4x4 Reticle3DTransMat =
	//    MyMath::MakeTranslateMatrix(worldTransform3DReticle_.translation_);
	//Vector3 offSet = {0, 0, 1.0f};
	//Matrix4x4 worMat = MyMath::MakeAffineMatrix(
	//    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_); 
	/*offSet = MyMath::TransformNormal(offSet, worldTransform_.matWorld_);
	offSet = MyMath::FVMultiply(kDistancePlayerTo3DReticle, MyMath::Normalize(offSet));
	worldTransform3DReticle_.translation_.x = offSet.x + worldTransform_.matWorld_.m[3][0];
	worldTransform3DReticle_.translation_.y = offSet.y + worldTransform_.matWorld_.m[3][1];
	worldTransform3DReticle_.translation_.z = offSet.z + worldTransform_.matWorld_.m[3][2];*/
	//worldTransform3DReticle_.UpdateMatrix();
	//Vector3 positionReticle;
	/*positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	Matrix4x4 matViewPort =
	    MyMath::MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort = MyMath::MMMultiply(
	    viewProjection.matView, MyMath::MMMultiply(viewProjection.matProjection, matViewPort));
	positionReticle = MyMath::TransformCoord(positionReticle, matViewProjectionViewPort);*/
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));



	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;


	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -500.0f, 500.0f);
	ImGui::End();


	



	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}



}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}




}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
}

void Player::Rotate() {
	//
	const float kRotSpeed = 0.02f;
	
	//
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed; 
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (!isControl_) {
		XINPUT_STATE joyState;

		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			isAttack = true;
		} else {
			isAttack = false;
		}
	} else if (isControl_) {
		if (input_->PushKey(DIK_SPACE)) {
			isAttack = true;
		} else {
			isAttack = false;
		}
	}

	if (isAttack) {

		const float kBulletSpeed = 1.0f;
		//Vector3     velocity(0, 0, kBulletSpeed);
		Vector3 velocity;
		Vector3 Reticle3DPos;
		Reticle3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
		Reticle3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
		Reticle3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

		velocity = Reticle3DPos - GetWorldPosition();
		velocity = MyMath::FVMultiply(kBulletSpeed, MyMath::Normalize(velocity));
		/*MyMath::TransformNormal(velocity, worldTransform_.matWorld_);*/

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	/*worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;*/

	/*if (worldTransform_.parent_) {
		
	}*/
	return worldPos;
}

void Player::OnCollision() {

}

void Player::MouseUpdate(ViewProjection& viewProjection) { 
	
	if(input_->TriggerKey(DIK_M)) {
		isControl_ = true;
		
	} else if (input_->TriggerKey(DIK_C)) {
		isControl_ = false;
	}

	if (isControl_) {
		 POINT mouseposition;
		
		//マウス座標(スクリーン)を取得する
		 GetCursorPos(&mouseposition);
		//クライアントエリア座標に変換する
		 HWND hwnd = WinApp::GetInstance()->GetHwnd();
		 ScreenToClient(hwnd, &mouseposition);

		//Vector2 spritePos = sprite2DReticle_->GetPosition();

		//マウス座標を2Dレティクルのスプライトに代入する
		 sprite2DReticle_->SetPosition(
		     {float(mouseposition.x),
		      float(mouseposition.y)} /*{spritePos.x + mouseposition.x, spritePos.y +
		      mouseposition.y}*/);
	}
	else if (!isControl_) {
		XINPUT_STATE joyState;
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 8.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 8.0f;

			sprite2DReticle_->SetPosition(spritePosition);
		}
	}
	




	Matrix4x4 viewPortMat = MyMath::MakeViewPortMatrix(
	    0, 0, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);


	Matrix4x4 matVPV = MyMath::MMMultiply(
	    viewProjection.matView, MyMath::MMMultiply(viewProjection.matProjection, viewPortMat)
		);

	Matrix4x4 matInverseVPV = MyMath::Inverse(matVPV);

	Vector3 posNear =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	//
	posNear = MyMath::TransformCoord(posNear, matInverseVPV);
	posFar = MyMath::TransformCoord(posFar, matInverseVPV);

	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = MyMath::Normalize(mouseDirection);

	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ =
	    MyMath::FVMultiply(kDistanceTestObject, mouseDirection) + posNear;

	worldTransform3DReticle_.UpdateMatrix();


	 ImGui::Begin("Player");
	 ImGui::Text(
	     "2DReticle:(%f, %f)", sprite2DReticle_->GetPosition().x,
	     sprite2DReticle_->GetPosition().y);
	 ImGui::Text("Near:(%+.2f,%+.2f,%+.2f,)", posNear.x, posNear.y, posNear.z);
	 ImGui::Text("Far:(%+.2f,%+.2f,%+.2f,)", posFar.x, posFar.y, posFar.z);
	 ImGui::Text(
	     "3DReticle:(%+.2f,%+.2f,%+.2f,)", worldTransform3DReticle_.translation_.x,
	     worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

	
	ImGui::End();

}
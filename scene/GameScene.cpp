#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

#pragma region
	// for (size_t i = 0; i < _countof(worldTransform_); i++) {
	//	// x,y,z方向のスケーリングを特定
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
	//	// x,y,z軸周りの回転角を設定
	//	worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
	//	//
	//	worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

	//	worldTransform_[i].Initialize();
	//}
#pragma endregion 100個のワールドトランスフォーム初期化

	//親(0番)
	worldTransform_[PartID::Root].Initialize();

	//子
	worldTransform_[PartID::Spine].translation_ = {0, 4.5f, 0};
	worldTransform_[PartID::Spine].parent_ = &worldTransform_[PartID::Root];
	worldTransform_[PartID::Spine].Initialize();
	//上半身
	worldTransform_[PartID::Chest].translation_ = {0, -4.5f, 0};
	worldTransform_[PartID::Chest].parent_ = &worldTransform_[PartID::Spine];
	worldTransform_[PartID::Chest].Initialize();

	worldTransform_[PartID::Head].translation_ = {0, 4.5f, 0};
	worldTransform_[PartID::Head].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::Head].Initialize();

	worldTransform_[PartID::ArmL].translation_ = {4.5f, 0, 0};
	worldTransform_[PartID::ArmL].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::ArmL].Initialize();

	worldTransform_[PartID::ArmR].translation_ = {-4.5f, 0, 0};
	worldTransform_[PartID::ArmR].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::ArmR].Initialize();

	//下半身
	worldTransform_[PartID::Hip].translation_ = {0, -9.0f, 0};
	worldTransform_[PartID::Hip].parent_ = &worldTransform_[PartID::Spine];
	worldTransform_[PartID::Hip].Initialize();

	worldTransform_[PartID::LegL].translation_ = {4.5f, -4.5f, 0};
	worldTransform_[PartID::LegL].parent_ = &worldTransform_[PartID::Hip];
	worldTransform_[PartID::LegL].Initialize();

	worldTransform_[PartID::LegR].translation_ = {-4.5f, -4.5f, 0};
	worldTransform_[PartID::LegR].parent_ = &worldTransform_[PartID::Hip];
	worldTransform_[PartID::LegR].Initialize();

	// viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	// viewProjection_.aspectRatio = 1.0f;

	// viewProjection_.nearZ = 52.0f;
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
}

void GameScene::Update() {
	XMFLOAT3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	const float kChestRotSpeed = 0.05f;

	const float kHipRotSpeed = 0.05f;

	if (input_->PushKey(DIK_J)) {
		worldTransform_[PartID::Hip].rotation_.y -= kHipRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartID::Hip].rotation_.y += kHipRotSpeed;
	}

	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartID::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransform_[PartID::Chest].rotation_.y += kChestRotSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	/*XMFLOAT3 move_eye = {0, 0, 0};

	XMFLOAT3 move_target = {0, 0, 0};

	const float kEyeSpeed = 0.2f;

	const float kTargetSpeed = 0.2f;

	const float kUpRotSpeed = 0.05f;

	if (input_->PushKey(DIK_W))
	{
	    move_eye = {0,0,kEyeSpeed};
	}
	else if (input_->PushKey(DIK_S))
	{
	    move_eye = {0,0,-kEyeSpeed};
	}
	if (input_->PushKey(DIK_LEFT))
	{
	    move_target = {-kTargetSpeed,0, 0};
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
	    move_target = {kTargetSpeed,0, 0};
	}

	if (input_->PushKey(DIK_SPACE))
	{
	    viewAngle += kUpRotSpeed;
	    viewAngle = fmodf(viewAngle, XM_2PI);
	}

	viewProjection_.eye.x += move_eye.x;
	viewProjection_.eye.y += move_eye.y;
	viewProjection_.eye.z += move_eye.z;



	viewProjection_.target.x += move_target.x;
	viewProjection_.target.y += move_target.y;
	viewProjection_.target.z += move_target.z;

	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
	*/

	worldTransform_[PartID::Root].translation_.x += move.x;
	worldTransform_[PartID::Root].translation_.y += move.y;
	worldTransform_[PartID::Root].translation_.z += move.z;

#pragma region
	//視野角、射影

	if (input_->PushKey(DIK_W)) {
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	} else if (input_->PushKey(DIK_S)) {
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	}

	if (input_->PushKey(DIK_UP)) {
		viewProjection_.nearZ += 0.1f;
	} else if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.nearZ -= 0.1f;
	}
#pragma endregion 2_3 射影変換

	worldTransform_[PartID::Root].UpdateMatrix();
	worldTransform_[PartID::Spine].UpdateMatrix();
	worldTransform_[PartID::Head].UpdateMatrix();
	worldTransform_[PartID::Chest].UpdateMatrix();
	worldTransform_[PartID::ArmL].UpdateMatrix();
	worldTransform_[PartID::ArmR].UpdateMatrix();
	worldTransform_[PartID::Hip].UpdateMatrix();
	worldTransform_[PartID::LegL].UpdateMatrix();
	worldTransform_[PartID::LegR].UpdateMatrix();
	// viewProjection_.UpdateMatrix();

#pragma region
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f%f%f)", worldTransform_[PartID::Root].translation_.x,
	  worldTransform_[PartID::Root].translation_.y, worldTransform_[PartID::Root].translation_.z);
#pragma endregion デバッグテキスト
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/*for (size_t i = 0; i < _countof(worldTransform_); i++) {
	    model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}*/
	// model_->Draw(worldTransform_[PartID::Root], viewProjection_, textureHandle_);
	// model_->Draw(worldTransform_[PartID::Spine], viewProjection_ , textureHandle_);
	model_->Draw(worldTransform_[PartID::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartID::LegR], viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

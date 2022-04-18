﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

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


	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		// x,y,z方向のスケーリングを特定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		// x,y,z軸周りの回転角を設定
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		//
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		worldTransform_[i].Initialize();
	}


	viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	
	//viewProjection_.aspectRatio = 1.0f;

	viewProjection_.nearZ = 52.0f;
	viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
}

void GameScene::Update() 
{
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

	if (input_->PushKey(DIK_W)) 
	{
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	} 
	else if (input_->PushKey(DIK_S)) 
	{
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	}

	if (input_->PushKey(DIK_UP)) 
	{
		viewProjection_.nearZ += 0.1f;
	} else if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.nearZ -= 0.1f;
	}

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	  viewProjection_.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

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

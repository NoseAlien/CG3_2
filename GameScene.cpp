﻿#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <imgui.h>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete objSkydome;
	delete objGround;
	delete objFighter;
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete modelSphere;
	delete camera;
	delete light;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

    // カメラ生成
	camera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight, input);

	// カメラ注視点をセット
	camera->SetTarget({0, 1, 0});
	camera->SetDistance(3.0f);

    // 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	light = LightGroup::Create();
	light->SetDirectionalLightColor(0,{ 1,1,1 });
	//light->SetDirectionalLightActive(0, true);
	//light->SetPointLightActive(0, true);
	//pointLightPos[0] = 0.5f;
	//pointLightPos[1] = 1.0f;
	//pointLightPos[2] = 0.0f;
	light->SetSpotLightActive(0, true);

	Object3d::SetLight(light);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);

	modelSphere = Model::CreateFromOBJ("sphere", true);
	objSphere = Object3d::Create();
	objSphere->SetModel(modelSphere);
	objFighter->SetPosition({ 1,0,0 });
	objSphere->SetPosition({ -1,1,0 });
}

void GameScene::Update()
{
	XMFLOAT3 rot = objSphere->GetRotation();
	rot.y += 1.0f;
	objSphere->SetRotation(rot);
	objFighter->SetRotation(rot);

	light->SetDirectionalLightActive(0, lightActive0);
	light->SetDirectionalLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	light->SetDirectionalLightColor(0, XMFLOAT3(lightColor0));

	light->SetDirectionalLightActive(1, lightActive1);
	light->SetDirectionalLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	light->SetDirectionalLightColor(1, XMFLOAT3(lightColor1));

	light->SetDirectionalLightActive(2, lightActive2);
	light->SetDirectionalLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	light->SetDirectionalLightColor(2, XMFLOAT3(lightColor2));

	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	light->SetSpotLightDir(0, XMVECTOR({spotLightDir[0], spotLightDir[1], spotLightDir[2], 0}));
	light->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	light->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	light->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	light->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));

	std::ostringstream debugstr;

	const XMFLOAT3& cameraPos = camera->GetEye();
	debugstr << "cameraPos("
		<< std::fixed << std::setprecision(2)
		<< cameraPos.x << ","
		<< cameraPos.y << ","
		<< cameraPos.z << ")";
	debugText.Print(debugstr.str(), 50, 130, 1.0f);


	camera->Update();
	light->Update();

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();
	objSphere->Update();

	debugText.Print("AD: move camera LeftRight", 50, 50, 1.0f);
	debugText.Print("WS: move camera UpDown", 50, 70, 1.0f);
	debugText.Print("ARROW: move camera FrontBack", 50, 90, 1.0f);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objSphere->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 1000));

	ImGui::Checkbox("dirLightActive0", &lightActive0);
	ImGui::InputFloat3("dirLightDir0", lightDir0);
	ImGui::ColorEdit3("dirLightColor0", lightColor0, ImGuiColorEditFlags_Float);

	ImGui::Checkbox("dirLightActive1", &lightActive1);
	ImGui::InputFloat3("dirLightDir1", lightDir1);
	ImGui::ColorEdit3("dirLightColor1", lightColor1, ImGuiColorEditFlags_Float);

	ImGui::Checkbox("dirLightActive2", &lightActive2);
	ImGui::InputFloat3("dirLightDir2", lightDir2);
	ImGui::ColorEdit3("dirLightColor2", lightColor2, ImGuiColorEditFlags_Float);

	ImGui::Checkbox("spotLightActive", &spotLightActive);
	ImGui::InputFloat3("spotLightDir", spotLightDir);
	ImGui::InputFloat3("spotLightPos", spotLightPos);
	ImGui::ColorEdit3("spotLightColor", spotLightColor, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("spotLightAtten", spotLightAtten);
	ImGui::InputFloat2("spotLightFactorAngle", spotLightFactorAngle);

	ImGui::End();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

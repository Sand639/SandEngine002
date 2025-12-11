//=======================================================
// ファイル名	: TestScene.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/27
// 更新日		: 2025/11/27
// 詳細			: テストシーンクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "TestScene.h"		// テストシーンクラス
#include "Entity.h"			// エンティティクラス
#include "Transform.h"		// トランスフォームコンポーネントクラス
#include "Debug.h"			// デバッグクラス
#include "Mesh.h"			// メッシュクラス
#include "MeshRenderer.h"	// メッシュレンダラクラス
#include "GameEngine.h"		// ゲームエンジンクラス
#include "MessageBox.h"		// メッセージボックスラッパークラス

/// <summary>
/// 初期化関数
/// </summary>
void TestScene::Init()
{
	Scene::Init();
}

/// ライフサイクル関数

/// <summary>
/// 初期化関数
/// </summary>
void TestScene::Awake()
{
	
	Scene::Awake();
}

/// <summary>
/// 開始処理関数
/// </summary>
void TestScene::Start()
{
	//テスト用のエンティティを生成
	m_testEntity = AddEntity<Entity>();
	//テスト用エンティティにTransformコンポーネントを追加
	std::shared_ptr<Transform> transform = m_testEntity->AddComponent<Transform>();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	transform->SetScale(1.0f, 1.0f, 1.0f);

	//テスト用エンティティにMeshRendererコンポーネントを追加
	std::shared_ptr<MeshRenderer> meshRenderer = m_testEntity->AddComponent<MeshRenderer>();

	std::vector<Vertex> vertices =
	{
		{ {  0.0f,  0.5f, 0.0f }, {1, 0, 0, 1},{0,0}, { 0, 0, 1 } }, // 上（赤）
		{{  0.5f, -0.5f, 0.0f }, {0, 1, 0, 1},{0,1}, { 0, 0, 1 } }, // 右下（緑）
		{ { -0.5f, -0.5f, 0.0f }, {0, 0, 1, 1},{1,0}, { 0, 0, 1 } }, // 左下（青）
		//{ {  1.0f,  1.0f, 0.0f }, {1, 1, 0, 1},{1,1}, { 0, 0, 1 } }
	};

	std::vector<UINT> indices = { 0, 1, 2 };

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	auto* renderer = GameEngine::GetInstance().GetRenderer();

	mesh->Init(renderer->GetDevice().Get(), vertices, indices);
	meshRenderer->SetMesh(mesh);

	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->LoadFromJson("Assets/Materials/Red.material.json");

	meshRenderer->SetMaterial(material);

	Scene::Start();
}

/// <summary>
/// 終了処理関数
/// </summary>
void TestScene::Uninit()
{
	
	Scene::Uninit();
}

/// <summary>
/// 更新関数
/// </summary>
void TestScene::Update()
{
	auto transform = m_testEntity->GetComponent<Transform>();
	if (transform)
	{
		// 毎フレーム Y 回転を少し足す（単位はラジアン想定）
		transform->Rotate(0.0f, 0.01f, 0.0f);

		//Debug::Log("Entity Position: ({:.2f}, {:.2f}, {:.2f})",
		//	transform->GetRotation().x,
		//	transform->GetRotation().y,
		//	transform->GetRotation().z);
	}

	Scene::Update();
}

/// <summary>
/// 固定更新関数
/// </summary>
void TestScene::FixedUpdate()
{

	Scene::FixedUpdate();
}

/// <summary>
/// 後更新関数
/// </summary>
void TestScene::LateUpdate()
{

	Scene::LateUpdate();
}

/// <summary>
/// 描画関数
/// </summary>
void TestScene::Draw()
{

	Scene::Draw();
}

/// <summary>
/// フレーム終了処理関数
/// </summary>
void TestScene::EndOfFrame()
{
	
	Scene::EndOfFrame();
}
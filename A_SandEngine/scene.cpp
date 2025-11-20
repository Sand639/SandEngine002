//=======================================================
// ファイル名	: Scene.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 更新日		: 2025/11/20
// 詳細			: シーンクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Scene.h"      // シーンクラス
#include <algorithm>    // std::remove_if

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="name">シーンの名前</param>
Scene::Scene(const std::string& name) : m_name(name)
{

}


/// ライフサイクル関数

/// <summary>
/// 初期化関数
/// </summary>
void Scene::Init()
{
	//エンティティの初期化
	for (auto& entity : m_entities)
	{
		entity->Init();
	}
}

/// <summary>
/// 終了処理関数
/// </summary>
void Scene::Uninit()
{
	//エンティティの終了処理
	for (auto& entity : m_entities)
	{
		entity->Uninit();
	}
	m_entities.clear();
}

/// <summary>
/// 更新関数
/// </summary>
void Scene::Update()
{
	//エンティティの更新処理
	for (auto& entity : m_entities)
	{
		entity->Update();
	}

	// 破棄フラグの立っている Entity をまとめて消す
	m_entities.erase(
		std::remove_if(m_entities.begin(), m_entities.end(),
			[](const std::shared_ptr<Entity>& e) { return e->OnDestroy(); }),
		m_entities.end()
	);

}

/// <summary>
/// 固定更新関数
/// </summary>
void Scene::FixedUpdate()
{

	//エンティティの固定更新処理
	for (auto& entity : m_entities)
	{
		entity->FixedUpdate();
	}
}

/// <summary>
/// 後更新関数
/// </summary>
void Scene::LateUpdate()
{

	//エンティティの後更新処理
	for (auto& entity : m_entities)
	{
		entity->LateUpdate();
	}
}

/// <summary>
/// 描画関数
/// </summary>
void Scene::Draw()
{

	//エンティティの描画処理
	for (auto& entity : m_entities)
	{
		entity->Draw();
	}
}


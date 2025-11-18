//=======================================================
// ファイル名	: Entity.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: エンティティの実装ファイル
//=======================================================
#include "Entity.h"

void Entity::Init()
{
	//コンポーネントの初期化
	for (auto& comp : m_components)
	{
		comp->Init();
	}
}

void Entity::Uninit()
{
	//コンポーネントの終了処理
	for (auto& comp : m_components)
	{
		comp->Uninit();
	}
}

void Entity::Update()
{

	if (!m_isActive) return;

	//コンポーネントの更新処理
	for (auto& comp : m_components)
	{
		comp->Update();
	}
}

void Entity::FixedUpdate()
{

	if (!m_isActive) return;

	//コンポーネントの固定更新処理
	for (auto& comp : m_components)
	{
		comp->FixedUpdate();
	}
}

void Entity::LateUpdate()
{

	if (!m_isActive) return;

	//コンポーネントの後更新処理
	for (auto& comp : m_components)
	{
		comp->LateUpdate();
	}
}

void Entity::Draw()
{

	if (!m_isActive) return;

	//コンポーネントの描画処理
	for (auto& comp : m_components)
	{
		comp->Draw();
	}
}


/// <summary>
/// 有効状態の設定
/// </summary>
/// <param name="_isActive">変更したい有効状態</param>
void Entity::SetActive(bool _isActive)
{
	//フラグが同じなら何もしない
	if (m_isActive == _isActive) return;

	//状態を変更
	m_isActive = _isActive;

	//変更フラグを立てる
	m_changeActive = true;

}

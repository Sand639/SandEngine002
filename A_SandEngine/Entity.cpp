//=======================================================
// ファイル名	: Entity.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/20
// 詳細			: エンティティの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Entity.h"

/// <summary>
/// 初期化関数
/// </summary>
void Entity::Init()
{
	//コンポーネントの初期化
	for (auto& comp : m_components)
	{
		comp->Init();
	}
}

/// <summary>
/// 終了処理関数
/// </summary>
void Entity::Uninit()
{
	//コンポーネントの終了処理
	for (auto& comp : m_components)
	{
		comp->Uninit();
	}
}

/// <summary>
/// 更新関数
/// </summary>
void Entity::Update()
{

	if (!m_isActive) return;

	//コンポーネントの更新処理
	for (auto& comp : m_components)
	{
		comp->Update();
	}
}

/// <summary>
/// 固定更新関数
/// </summary>
void Entity::FixedUpdate()
{

	if (!m_isActive) return;

	//コンポーネントの固定更新処理
	for (auto& comp : m_components)
	{
		comp->FixedUpdate();
	}
}

/// <summary>
/// 後更新関数
/// </summary>
void Entity::LateUpdate()
{

	if (!m_isActive) return;

	//コンポーネントの後更新処理
	for (auto& comp : m_components)
	{
		comp->LateUpdate();
	}
}

/// <summary>
/// 描画関数
/// </summary>
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
/// 破棄処理関数
/// </summary>
/// <returns>破棄されたかどうか</returns>
bool Entity::OnDestroy()
{
	//破棄フラグが立っていたら破棄処理を行う
	if (m_isDestroy) {
		Uninit();	//終了処理
		return true;//破棄対象
	}
	return false;	//破棄対象外
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

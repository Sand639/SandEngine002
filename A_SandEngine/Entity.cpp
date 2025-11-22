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
void Entity::Awake()
{
	//コンポーネントの初期化
	for (auto& comp : m_components)
	{
		comp->Awake();
	}
}

/// <summary>
/// 開始処理関数
/// </summary>
void Entity::Start()
{
	//コンポーネントの初期化
	for (auto& comp : m_components)
	{
		comp->Start();
	}

	//スタート保留フラグを下ろす
	m_isStartPending = false;
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
/// フレーム終了処理関数
/// </summary>
void Entity::EndOfFrame()
{
	if (!m_isActive) return;

	//コンポーネントのフレーム終了処理
	for (auto& comp : m_components)
	{
		comp->EndOfFrame();
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
/// 有効状態更新関数
/// </summary>
/// <remarks>
/// SceneクラスのUpdate関数内と最終処理関数内で呼び出される
/// </remarks>
void Entity::UpdateActiveState()
{
	// 有効状態の変更があったか確認
	if (m_changeActive)
	{
		// 有効状態が変化した場合の処理
		if (m_isActive)
		{
			// 有効化された場合の処理
			OnEnable();
		}
		else
		{
			// 無効化された場合の処理
			OnDisable();
		}

		// 変更フラグをリセット
		m_changeActive = false;		

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

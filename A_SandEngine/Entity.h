//=======================================================
// ファイル名	: Entity.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/20
// 詳細			: エンティティのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <string>		// std::string
#include <memory>		// std::shared_ptr
#include <vector>		// std::vector
#include <type_traits>	// std::is_base_of

#include "Component.h"	// コンポーネント基底クラス

/// <summary>
/// エンティティクラス
/// </summary>
/// <remarks>
/// ゲーム内のエンティティの基底クラス
/// </remarks>
class Entity : public std::enable_shared_from_this<Entity>
{
protected:

	//このエンティティの名前
	std::string m_name = "entity";

	//このエンティティのID(識別子)
	int m_id = 0;

	//このエンティティが有効かどうか
	bool m_isActive = true;
	//このオブジェクトを変更したかどうか
	bool m_changeActive = false;

	//このエンティティが破棄されるかどうか
	bool m_isDestroy = false;

	//スタート処理が保留されているかどうか
	bool m_isStartPending = true;
	
	

	//コンポーネント
	std::vector<std::shared_ptr<Component>> m_components;

public:

	//コンストラクタ
	Entity() = default;
	//デストラクタ
	virtual ~Entity() = default;

	//ライフサイクル関数

	virtual void Awake();
	virtual void Start();
	virtual void Uninit();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Draw();
	virtual void EndOfFrame();


	bool OnDestroy();			//破棄処理関数
	void UpdateActiveState();	//有効状態更新関数

	//セッター

	void SetName(const std::string& name) { m_name = name; }		//名前の設定
	void SetID(int id) { m_id = id; }	//IDの設定
	void SetActive(bool _isActive);		//有効状態の設定
	void SetDestroy(bool _isDestroy) { m_isDestroy = _isDestroy; }	//破棄状態の設定

	//ゲッター

	const std::string& GetName() const { return m_name; }	//名前の取得
	int GetID() const { return m_id; }						//IDの取得
	bool GetIsActive() const { return m_isActive; }			//有効状態の取得
	bool GetChangeActive() const { return m_changeActive; }	//有効状態変更フラグの取得
	bool GetIsDestroy() const { return m_isDestroy; }		//破棄状態の取得
	bool GetIsStartPending() const { return m_isStartPending; } //スタート保留状態の取得

	// --- コンポーネント管理 ---

	//コンポーネント追加関数
	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);

	// 特定の型のコンポーネントを取得する関数
	template <typename T>
	std::shared_ptr<T> GetComponent() const;

protected:

	virtual void OnEnable() {}
	virtual void OnDisable() {}
};

/// <summary>
/// コンポーネント追加関数の実装
/// </summary>
/// <typeparam name="T">追加するコンポーネントの型</typeparam>
/// <typeparam name="Args">コンポーネントのコンストラクタ引数の型</typeparam>
/// <param name="args">コンポーネントのコンストラクタ引数</param>
/// <returns>追加したコンポーネントの共有ポインタ</returns>
template<typename T, typename ...Args>
inline std::shared_ptr<T> Entity::AddComponent(Args && ...args)
{
	// TがComponentの派生クラスであることを確認
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

	// コンポーネントを生成
	std::shared_ptr<T> newComp = std::make_shared<T>(std::forward<Args>(args)...);

	// 親ポインタを設定
	newComp->SetOwner(shared_from_this());

	// リストに追加
	m_components.push_back(newComp);

	newComp->Awake(); // Awakeを呼び出す

	return newComp; // 追加したコンポーネントを返す
}

/// <summary>
/// 特定の型のコンポーネントを取得する関数の実装
/// </summary>
/// <typeparam name="T">取得するコンポーネントの型</typeparam>
/// <returns>取得したコンポーネントの共有ポインタ、見つからなかった場合はnullptr</returns>
template<typename T>
inline std::shared_ptr<T> Entity::GetComponent() const
{
	for (const auto& comp : m_components)
	{
		if (std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(comp))
		{
			return casted;  // 見つかった場合は返す
		}
	}
	return nullptr; // 見つからなかった場合は nullptr を返す
}

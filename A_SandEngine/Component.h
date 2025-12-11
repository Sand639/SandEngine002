//=======================================================
// ファイル名	: Component.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: コンポーネント基底クラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <memory>	// std::shared_ptr, std::weak_ptr

//=======================================================
// 前方宣言
//=======================================================
class Entity;

//=======================================================
/// クラス宣言
//=======================================================

/// <summary>
/// コンポーネント基底クラス
/// </summary>
/// <remarks>
/// 各コンポーネントは Entity にアタッチされることで機能を発揮する
/// </remarks>
class Component
{
private:
	//このコンポーネントを所持してるEntityクラスへの弱参照
    std::weak_ptr<Entity> m_owner;

public:
    //コンストラクタ
	Component() = default;
    // デストラクタ
    virtual ~Component() = default;

    // ライフサイクル

    virtual void Awake()        {}
    virtual void Start()        {}
    virtual void Uninit()       {}
    virtual void Update()       {}
    virtual void FixedUpdate()  {}
    virtual void LateUpdate()   {}
    virtual void Draw()         {}
    virtual void EndOfFrame()  {}

	// --- セッター ---

    // 親 Entity を設定する
    void SetOwner(const std::shared_ptr<Entity>& owner) { m_owner = owner; }

    // --- ゲッター ---
     
	// 所有者への弱参照を取得します
    std::weak_ptr<Entity> GetOwnerWeak() const {
        return m_owner;
    }

	// 所有者への共有ポインタを取得します
    std::shared_ptr<Entity> GetOwner() const {
        return m_owner.lock();
    }
};
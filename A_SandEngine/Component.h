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
    Entity* m_owner = nullptr;   // 所有権は持たない

public:
    //コンストラクタ
	Component() = default;
    // デストラクタ
    virtual ~Component() = default;

    // ライフサイクル

    virtual void Init()         {}
    virtual void Uninit()       {}
    virtual void Update()       {}
    virtual void FixedUpdate()  {}
    virtual void LateUpdate()   {}
    virtual void Draw()         {}

    // 親 Entity を設定する
    void SetOwner(Entity* owner) { m_owner = owner; }

    // 親 Entity を取得する
    Entity* GetOwner() const { return m_owner; }
};
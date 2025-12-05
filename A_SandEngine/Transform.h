//=======================================================
// ファイル名	: Transform.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: Transformコンポーネントクラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include "Component.h"		// Component基底クラス
#include <DirectXMath.h>	// DirectXMath (数学ライブラリ)

//=======================================================
// クラス宣言
//=======================================================

/// <summary>
/// Transformコンポーネントクラス
/// </summary>
/// <remarks>
/// 位置、回転、スケールなどの変換情報を管理するコンポーネント
/// </remarks>
class Transform : public Component
{
private:
    // 変換情報
    DirectX::XMFLOAT3 m_position; // 位置 (X, Y, Z)
    DirectX::XMFLOAT3 m_rotation; // 回転 (ピッチ、ヨー、ロールのオイラー角)
    DirectX::XMFLOAT3 m_scale;    // スケール (X, Y, Z)

    // 行列計算結果
    mutable DirectX::XMMATRIX m_worldMatrix; // ワールド行列 (キャッシュ用)
    mutable bool m_isDirty;         // 変更フラグ (最適化のため)

public:
    // コンストラクタ
    Transform();
    // デストラクタ
    virtual ~Transform() = default;

    void Translate(float dx, float dy, float dz);
    void Rotate(float dx, float dy, float dz);

    // --- ゲッター・セッター ---

    // 位置
    const DirectX::XMFLOAT3& GetPosition() const { return m_position; }
    void SetPosition(const DirectX::XMFLOAT3& position) { m_position = position; m_isDirty = true; }
    void SetPosition(float x, float y, float z)
    {
        m_position = DirectX::XMFLOAT3(x, y, z);
        m_isDirty = true;
    }

    // 回転 (ここではオイラー角を想定)
    const DirectX::XMFLOAT3& GetRotation() const { return m_rotation; }
    void SetRotation(const DirectX::XMFLOAT3& rotation) { m_rotation = rotation; m_isDirty = true; }
    void SetRotation(float x, float y, float z)
    {
        m_rotation = DirectX::XMFLOAT3(x, y, z);
        m_isDirty = true;
    }

    // スケール
    const DirectX::XMFLOAT3& GetScale() const { return m_scale; }
    void SetScale(const DirectX::XMFLOAT3& scale) { m_scale = scale; m_isDirty = true; }
    void SetScale(float x, float y, float z)
    {
        m_scale = DirectX::XMFLOAT3(x, y, z);
        m_isDirty = true;
    }

    // ワールド行列の取得
    const DirectX::XMMATRIX& GetWorldMatrix() const;

};


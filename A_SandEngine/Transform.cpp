//=======================================================
// ファイル名	: Transform.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: Transformコンポーネントクラスのソースファイル
//=======================================================
#include "Transform.h"  // Transformコンポーネントクラス

//=======================================================
// 名前空間の使用
//=======================================================
using namespace DirectX;    // DirectXMath名前空間を使用

//コンストラクタ
Transform::Transform() :
	m_position(0.0f, 0.0f, 0.0f),	// 初期位置を原点に設定
	m_rotationEuler(0.0f, 0.0f, 0.0f),	// 初期回転をゼロに設定
	m_scale(1.0f, 1.0f, 1.0f),		// 初期スケールを1に設定
	m_isDirty(true)					// 初期状態では変更フラグを立てる
{
	// 初期状態ではワールド行列を単位行列に設定
	m_worldMatrix = XMMatrixIdentity();

    // クォータニオンの単位回転（回転なし）
    m_rotationQuat = DirectX::XMFLOAT4(0, 0, 0, 1);
}


/// <summary>
/// 平行移動関数
/// </summary>
void Transform::Translate(float dx, float dy, float dz)
{
    m_position.x += dx;
    m_position.y += dy;
    m_position.z += dz;
	m_isDirty = true; // 位置が変わったのでフラグを立てる
}

/// <summary>
/// 回転関数
/// </summary>
void Transform::Rotate(float dx, float dy, float dz)
{
    float x = m_rotationEuler.x + dx;
    float y = m_rotationEuler.y + dy;
    float z = m_rotationEuler.z + dz;

    SetRotation(x, y, z);

}

/// <summary>
/// ワールド行列の取得
/// </summary>
const XMMATRIX& Transform::GetWorldMatrix() const
{
    // 変更フラグが立っている場合のみ再計算する
    if (m_isDirty)
    {
        // スケール行列 (S) を作成
        XMMATRIX S = XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
        
		// 回転行列 (R) を作成
        XMVECTOR q = XMLoadFloat4(&m_rotationQuat);
        XMMATRIX R = XMMatrixRotationQuaternion(q);

        // 移動行列 (T) を作成
        XMMATRIX T = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));

        // ワールド行列 (W) を計算: W = S * R * T の順が一般的
        // DirectXMathでは行ベクトルと列ベクトルの違いにより、計算順序を調整することがあります
        m_worldMatrix = S * R * T;

        // 再計算が完了したのでフラグを降ろす
        m_isDirty = false;
    }

    return m_worldMatrix;   // キャッシュされたワールド行列を返す

}

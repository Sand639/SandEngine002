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
	DirectX::XMFLOAT3 m_position;       // 位置 (X, Y, Z)
	DirectX::XMFLOAT3 m_rotationEuler;  // 回転 (ピッチ、ヨー、ロールのオイラー角)
	DirectX::XMFLOAT3 m_scale;          // スケール (X, Y, Z)

	DirectX::XMFLOAT4 m_rotationQuat; // 回転 (クォータニオン)

	// 行列計算結果
	mutable DirectX::XMMATRIX m_worldMatrix; // ワールド行列 (キャッシュ用)
	mutable bool m_isDirty;         // 変更フラグ (最適化のため)

public:
	// コンストラクタ
	Transform();
	// デストラクタ
	virtual ~Transform() = default;

	// コンポーネントの型名を取得する関数
	const char* GetTypeName() const override { return "Transform"; }

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
	const DirectX::XMFLOAT3& GetRotationRadians() const { return m_rotationEuler; }
	void SetRotationRadians(float rx, float ry, float rz)
	{
		m_rotationEuler = { rx, ry, rz };
		auto q = DirectX::XMQuaternionRotationRollPitchYaw(rx, ry, rz);
		DirectX::XMStoreFloat4(&m_rotationQuat, q);
		m_isDirty = true;
	}
	void SetRotationRadians(DirectX::XMFLOAT3 r)
	{
		m_rotationEuler = { r.x, r.y, r.z };
		auto q = DirectX::XMQuaternionRotationRollPitchYaw(r.x, r.y, r.z);
		DirectX::XMStoreFloat4(&m_rotationQuat, q);
		m_isDirty = true;
	}

	// デグリー角
	DirectX::XMFLOAT3 GetRotationDegrees() const
	{
		return {
			m_rotationEuler.x * 180.0f / 3.1415926535f,
			m_rotationEuler.y * 180.0f / 3.1415926535f,
			m_rotationEuler.z * 180.0f / 3.1415926535f
		};
	}
	void SetRotationDegrees(float dx, float dy, float dz)
	{
		SetRotationRadians(
			dx * 3.1415926535f / 180.0f,
			dy * 3.1415926535f / 180.0f,
			dz * 3.1415926535f / 180.0f
		);
	}
	void SetRotationDegrees(const DirectX::XMFLOAT3& d)
	{
		SetRotationRadians(
			d.x * 3.1415926535f / 180.0f,
			d.y * 3.1415926535f / 180.0f,
			d.z * 3.1415926535f / 180.0f
		);
	}

	// スケール
	const DirectX::XMFLOAT3& GetScale() const { return m_scale; }
	void SetScale(const DirectX::XMFLOAT3& scale) { m_scale = scale; m_isDirty = true; }
	void SetScale(float x, float y, float z)
	{
		m_scale = DirectX::XMFLOAT3(x, y, z);
		m_isDirty = true;
	}

	//クォータニオン
	const DirectX::XMFLOAT4& GetRotationQuat() const { return m_rotationQuat; }

	// ワールド行列の取得
	const DirectX::XMMATRIX& GetWorldMatrix() const;

};


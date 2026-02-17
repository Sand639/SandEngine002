//=======================================================
// ファイル名	: Camera.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/29
// 更新日		: 2026/01/29
// 詳細			: カメラのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include "Component.h"
#include <DirectXMath.h>
#include <memory>

//=======================================================
// 前方宣言
//=======================================================
class Transform;
class Scene;


/// <summary>
/// カメラコンポーネント
/// </summary>
/// <remarks>
/// カメラの設定を管理するコンポーネント
/// </remarks>
class Camera : public Component
{
private:
	bool  m_isMain = true;          // メインカメラかどうか
    float m_fovDegrees = 60.0f;     // 視野角
    float m_nearZ = 0.1f;            // ニアクリップ面
    float m_farZ = 100.0f;           // ファークリップ面

public:
	Camera() = default;             // コンストラクタ
	virtual ~Camera() = default;    // デストラクタ

	// コンポーネントの型名を取得する関数
    const char* GetTypeName() const override { return "Camera"; }

	// --- セッター / ゲッター ---

	// メインカメラかどうかの設定 / 取得
    void SetMain(bool v) { m_isMain = v; }
    bool IsMain() const { return m_isMain; }

	// 視野角の設定 / 取得
    void SetFovDegrees(float v) { m_fovDegrees = v; }
    float GetFovDegrees() const { return m_fovDegrees; }

	// ニアクリップ面の設定 / 取得
    void SetNearZ(float v) { m_nearZ = v; }
    float GetNearZ() const { return m_nearZ; }

	// ファークリップ面の設定 / 取得
    void SetFarZ(float v) { m_farZ = v; }
    float GetFarZ() const { return m_farZ; }

    // Transform から View を作る（左手座標 +Z forward）
    DirectX::XMMATRIX GetViewMatrix() const;

    // 画面サイズから Projection を作る
    DirectX::XMMATRIX GetProjectionMatrix(float aspect) const;

	static std::shared_ptr<Camera> FindMainCamera(const std::shared_ptr<Scene>& scene);
};

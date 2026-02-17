//=======================================================
// ファイル名	: Camera.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/29
// 更新日		: 2026/01/29
// 詳細			: カメラの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include <DirectXMath.h>
#include "Scene.h"

using namespace DirectX;

XMMATRIX Camera::GetViewMatrix() const
{
	// 所有者(Entity)を取得
    auto owner = GetOwner();
    if (!owner) return XMMatrixIdentity();

	// Transform コンポーネントを取得
    auto t = owner->GetComponent<Transform>();
    if (!t) return XMMatrixIdentity();

	// 位置を取得
    const auto posF = t->GetPosition();
    const XMVECTOR pos = XMVectorSet(posF.x, posF.y, posF.z, 1.0f);

    // Transform は Quaternion を保持してるので、それを使って forward/up を回転させる
    const auto rotQ = t->GetRotationQuat();
    const XMVECTOR q = XMLoadFloat4(&rotQ);

	// 回転を適用した各軸方向ベクトルを取得
    const XMVECTOR forward = XMVector3Normalize(XMVector3Rotate(XMVectorSet(0, 0, 1, 0), q)); // +Z
    const XMVECTOR up = XMVector3Normalize(XMVector3Rotate(XMVectorSet(0, 1, 0, 0), q)); // +Y

    // LookToLH: pos + forward の方向を見る（左手座標）
    return XMMatrixLookToLH(pos, forward, up);
}

XMMATRIX Camera::GetProjectionMatrix(float aspect) const
{
    const float fovRad = XMConvertToRadians(m_fovDegrees);
    return XMMatrixPerspectiveFovLH(fovRad, aspect, m_nearZ, m_farZ);
}

std::shared_ptr<Camera> Camera::FindMainCamera(const std::shared_ptr<Scene>& scene)
{
	if (!scene) return nullptr; // シーンが無効なら nullptr を返す

	// シーン内の全エンティティを調べる
    for (auto& e : scene->GetEntities())
    {
		if (!e) continue;   // エンティティが無効ならスキップ
		auto cam = e->GetComponent<Camera>();   // Camera コンポーネントを取得
		if (cam && cam->IsMain()) return cam;   // メインカメラなら返す
    }

    return nullptr;
}

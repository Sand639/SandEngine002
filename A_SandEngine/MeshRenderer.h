//=======================================================
// ファイル名	: MeshRenderer.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: メッシュレンダラーのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include "Component.h"		// Component基底クラス
#include "Mesh.h"			// メッシュクラス
#include "Material.h"		// マテリアルクラス

//=======================================================
// クラス定義
//=======================================================

/// <summary>
/// メッシュレンダラー
/// </summary>
/// <remarks>
/// メッシュとマテリアルを持ち、描画を行うコンポーネント
/// </remarks>
class MeshRenderer : public Component
{
private:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;

public:

	// コンストラクタ・デストラクタ
	MeshRenderer() = default;
	MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	~MeshRenderer() = default;

	//セッター
	void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
	void SetMaterial(std::shared_ptr<Material> material) { m_material = material; }

	// ライフサイクル関数

	// 開始関数
	void Start() override;

	// 描画関数
	void Draw() override;

};

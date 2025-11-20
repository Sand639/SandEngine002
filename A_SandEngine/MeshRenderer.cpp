//=======================================================
// ファイル名	: MeshRenderer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: メッシュレンダラーの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "MeshRenderer.h"
#include "Renderer.h"
#include "GameEngine.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="mesh">メッシュポインタ</param>
/// <param name="material">マテリアルポインタ</param>
MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
    : m_mesh(mesh), m_material(material)
{
}

/// <summary>
/// 描画関数
/// </summary>
void MeshRenderer::Draw()
{
	// デバイスコンテキストを取得
    auto context = GameEngine::GetInstance().GetRenderer()->GetDeviceContext();

    // マテリアルを適用
    m_material->Apply(context.Get());

    // メッシュを描画
    m_mesh->Draw(context.Get());
}
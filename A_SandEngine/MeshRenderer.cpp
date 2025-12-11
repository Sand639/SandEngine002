//=======================================================
// ファイル名	: MeshRenderer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 更新日		: 2025/12/04
// 詳細			: メッシュレンダラーの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "MeshRenderer.h"   // メッシュレンダラークラス
#include "Renderer.h"       // レンダラークラス
#include "GameEngine.h"     // ゲームエンジンクラス
#include "Debug.h"          // デバッグクラス    
#include "Transform.h"      // トランスフォームコンポーネントクラス

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="mesh">メッシュ</param>
/// <param name="material">マテリアル</param>
MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    : m_mesh(mesh), m_material(material)
{

}

void MeshRenderer::Start()
{

}

/// <summary>
/// 描画関数
/// </summary>
void MeshRenderer::Draw()
{
	// デバイスコンテキストを取得
    auto context = GameEngine::GetInstance().GetRenderer()->GetDeviceContext();

    if (!context)
    {
        Debug::LogWarning("MeshRenderer::Draw - デバイスコンテキストが取得できません");
		return;
    }

    if(!m_mesh)
    {
        Debug::LogWarning("MeshRenderer::Draw - メッシュルが設定されていません");
        return;
	}

    if (!m_material)
    {
        Debug::LogWarning("MeshRenderer::Draw - マテリアルが設定されていません");
        return;
    }

    auto transform = GetOwner()->GetComponent<Transform>();
    
    if (!transform)
    {
        Debug::LogWarning("MeshRenderer::Draw - Transformが設定されていません");
        return;
    }

	// ワールド行列をマテリアルに設定
	GameEngine::GetInstance().GetRenderer()->SetWorldMatrix(transform->GetWorldMatrix());

    // マテリアルを適用
    m_material->Apply();

    // メッシュを描画
    m_mesh->Draw(context.Get());
}
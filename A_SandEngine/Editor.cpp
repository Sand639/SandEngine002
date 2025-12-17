//=======================================================
// ファイル名	: Editor.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/17
// 詳細			: エディタクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Editor.h"         // エディタクラス
#include "ImGuiLayer.h"		// ImGuiレイヤークラス
#include "imgui.h"			// ImGui本体
#include "GameEngine.h"		// ゲームエンジンクラス
#include "Input.h"			// 入力管理クラス

/// <summary>
/// ヒエラルキーウィンドウ描画関数
/// </summary>
/// <param name="scene">現在のシーン</param>
/// <remarks>
/// シーン内のエンティティをリスト表示します。
/// </remarks>
void Editor::DrawHierarchy(const std::shared_ptr<Scene>& scene)
{

	if (!m_showHierarchy) return;	// ヒエラルキーウィンドウが非表示の場合は描画しない

	// ヒエラルキーウィンドウの開始
	ImGui::Begin(IMGUI_U8("ヒエラルキー"), &m_showHierarchy);

	const auto& entities = scene->GetEntities();	// シーン内のエンティティを取得
	for (const auto& e : entities)
	{
		// エンティティ名をリスト表示
		bool selected = (m_selectedEntity == e);
		if (ImGui::Selectable(e->GetName().c_str(), selected))
			m_selectedEntity = e;
	}

	ImGui::End();

}

/// <summary>
/// インスペクターウィンドウ描画関数
/// </summary>
/// <remarks>
/// 選択されたエンティティの詳細情報を表示します。
/// </remarks>
void Editor::DrawInspector()
{

	if (!m_showInspector) return;

	ImGui::Begin(IMGUI_U8("インスペクタ"));
	if (m_selectedEntity)
	{
		ImGui::Text("Name: %s", m_selectedEntity->GetName().c_str(), &m_showInspector);
	}
	else
	{
		ImGui::Text(IMGUI_U8("未選択"));
	}


	ImGui::End();

}

void Editor::Init()
{
	auto m_window = GameEngine::GetInstance().GetWindow();      // ウィンドウ取得
	auto m_renderer = GameEngine::GetInstance().GetRenderer();  // レンダラー取得

	// ImGuiレイヤーの初期化
	ImGuiLayer::Init(
		m_window->GetHandle(),
		m_renderer->GetDevice().Get(),
		m_renderer->GetDeviceContext().Get()
	);

	m_doneUninit = false;    // 解放済みフラグをリセット
}

void Editor::Update()
{

	if (Input::GetKeyTrigger(VK_F1))
	{
		m_visible = !m_visible;
	}

}

void Editor::Draw(const std::shared_ptr<Scene>& scene)
{

	if (!m_visible) return;

	// エディタの描画開始
	ImGuiLayer::BeginOfDraw();

	DrawHierarchy(scene);		// ヒエラルキーウィンドウ描画
	DrawInspector();			// インスペクターウィンドウ描画

	// ImGui描画終了
	ImGuiLayer::EndOfDraw();
}

void Editor::Uninit()
{
	// 二重解放防止
	if (m_doneUninit) return;
	m_doneUninit = true;    // 解放済みに設定

	ImGuiLayer::Uninit();	// ImGuiレイヤーの終了処理
}
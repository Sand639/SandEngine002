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
#include "Transform.h"		// トランスフォームコンポーネントクラス
#include "SceneSerializer.h"
#include "MeshRenderer.h"	// メッシュレンダラーコンポーネントクラス

static std::shared_ptr<Transform> FindTransform(const std::shared_ptr<Entity>& e)
{
	for (const auto& c : e->GetComponents())
	{
		auto t = std::dynamic_pointer_cast<Transform>(c);
		if (t) return t;
	}
	return nullptr;
}

static std::shared_ptr<MeshRenderer> FindMeshRenderer(const std::shared_ptr<Entity>& e)
{
	for (const auto& c : e->GetComponents())
	{
		auto mr = std::dynamic_pointer_cast<MeshRenderer>(c);
		if (mr) return mr;
	}
	return nullptr;
}

static int FindIndexBlend(BLENDSTATE v)
{
	const BLENDSTATE values[] = {
		BLENDSTATE_NONE, BLENDSTATE_ALFA, BLENDSTATE_ADD, BLENDSTATE_SUB, BLENDSTATE_MASK
	};
	for (int i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i)
		if (values[i] == v) return i;
	return 1; // ALFA
}

static int FindIndexShader(SHADERMODE v)
{
	const SHADERMODE values[] = { SHADERMODE_COLOR, SHADERMODE_TEXTURE, SHADERMODE_LIGHTING };
	for (int i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i)
		if (values[i] == v) return i;
	return 0;
}


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

		// コンポーネント一覧
		ImGui::Separator();
		ImGui::Text(IMGUI_U8("コンポーネント"));

		// コンポーネントをリスト表示
		const auto& comps = m_selectedEntity->GetComponents();
		for (const auto& c : comps)
		{
			if (!c) continue;
			ImGui::BulletText("%s", c->GetTypeName());

		}

		auto t = FindTransform(m_selectedEntity);
		if (t)
		{
			ImGui::Separator();
			ImGui::Text("Transform");

			auto pos = t->GetPosition();
			float p[3] = { pos.x, pos.y, pos.z };
			if (ImGui::DragFloat3("Position", p, 0.1f))
			{
				t->SetPosition({ p[0], p[1], p[2] });
			}

			auto rot = t->GetRotationDegrees();
			float r[3] = { rot.x, rot.y, rot.z };
			if (ImGui::DragFloat3("Rotation", r, 0.01f))
			{
				t->SetRotationDegrees({ r[0], r[1], r[2] });
			}

			auto scl = t->GetScale();
			float s[3] = { scl.x, scl.y, scl.z };
			if (ImGui::DragFloat3("Scale", s, 0.1f))
			{
				t->SetScale({ s[0], s[1], s[2] });
			}
		}

		if (m_lastScene)
		{
			ImGui::Separator();
			if (ImGui::Button(IMGUI_U8("Save Scene")))
			{
				SceneSerializer::Save(*m_lastScene, "Assets/Scenes/TestScene.json");
			}
		}

		auto mr = FindMeshRenderer(m_selectedEntity);
		if (mr)
		{
			auto mat = mr->GetMaterial();

			ImGui::Separator();
			ImGui::Text("MeshRenderer");

			if (mat)
			{
				ImGui::Text("Material");

				// --- ShaderMode ---
				{
					static const char* labels[] = { "Color", "Texture", "Lighting" };
					const SHADERMODE values[] = { SHADERMODE_COLOR, SHADERMODE_TEXTURE, SHADERMODE_LIGHTING };

					int idx = FindIndexShader(mat->GetShaderMode());
					if (ImGui::Combo("ShaderMode", &idx, labels, IM_ARRAYSIZE(labels)))
					{
						mat->SetShader(values[idx]);
					}
				}

				// --- BlendState ---
				{
					static const char* labels[] = { "None", "Alpha", "Add", "Sub", "Mask" };
					const BLENDSTATE values[] = {
						BLENDSTATE_NONE, BLENDSTATE_ALFA, BLENDSTATE_ADD, BLENDSTATE_SUB, BLENDSTATE_MASK
					};

					int idx = FindIndexBlend(mat->GetBlendState());
					if (ImGui::Combo("BlendState", &idx, labels, IM_ARRAYSIZE(labels)))
					{
						mat->SetBlendState(values[idx]);
					}
				}

				// --- BaseColor (RGBA) ---
				{
					auto c = mat->GetBaseColor(); // XMFLOAT4
					float col[4] = { c.x, c.y, c.z, c.w };
					if (ImGui::ColorEdit4("BaseColor", col, ImGuiColorEditFlags_Uint8))
					{
						mat->SetBaseColor({ col[0], col[1], col[2], col[3] });
					}
				}

				// --- Emissive (RGB) ---
				{
					auto e = mat->GetEmissive(); // XMFLOAT3
					float col[3] = { e.x, e.y, e.z };
					if (ImGui::ColorEdit3("Emissive", col, ImGuiColorEditFlags_Uint8))
					{
						mat->SetEmissive({ col[0], col[1], col[2] });
					}
				}

				// --- Metallic / Roughness ---
				{
					float m = mat->GetMetallic();
					float r = mat->GetRoughness();

					if (ImGui::SliderFloat("Metallic", &m, 0.0f, 1.0f))
						mat->SetMetallic(m);

					if (ImGui::SliderFloat("Roughness", &r, 0.0f, 1.0f))
						mat->SetRoughness(r);
				}

				// --- Save Material ---
				static bool s_saveOk = true;
				if (ImGui::Button("Save Material"))
				{
					const auto& path = mr->GetMaterialPath();
					if (!path.empty())
						s_saveOk = mat->SaveToJson(path);
					else
						s_saveOk = false;
				}
				ImGui::Text("MaterialPath: %s", mr->GetMaterialPath().c_str());
				ImGui::Text("Save Result: %s", s_saveOk ? "OK" : "FAILED");
			}
		}



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

	m_lastScene = scene;	// 最後に受け取ったシーンを保存

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
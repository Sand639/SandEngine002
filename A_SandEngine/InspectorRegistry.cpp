//=======================================================
// ファイル名	: InspectorRegistry.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/28
// 詳細		    : インスペクタ描画用のソースファイル
//=======================================================

//=======================================================
// インクルードファイル
//=======================================================
#include "InspectorRegistry.h"	// インスペクタ描画用ヘッダファイル

#include <unordered_map>		// std::unordered_map
#include <functional>			// std::function

#include "imgui.h"				// ImGui本体

#include "Renderer.h"			// レンダラクラス

#include "Entity.h"				// エンティティクラス
#include "Component.h"			// コンポーネントクラス
#include "Transform.h"			// トランスフォームコンポーネントクラス
#include "MeshRenderer.h"		// メッシュレンダラーコンポーネントクラス
#include "Material.h"			// マテリアルクラス


/// <summary>
/// エンティティからTransformコンポーネントを探す
/// </summary>
/// <param name="e">エンティティ</param>
/// <returns>Transformコンポーネントの共有ポインタ、見つからなければnullptr</returns>
static std::shared_ptr<Transform> FindTransform(const std::shared_ptr<Entity>& e)
{
	for (const auto& c : e->GetComponents())
	{
		auto t = std::dynamic_pointer_cast<Transform>(c);
		if (t) return t;
	}
	return nullptr;
}

/// <summary>
/// エンティティからMeshRendererコンポーネントを探す
/// </summary>
/// <param name="e">エンティティ</param>
/// <returns>MeshRendererコンポーネントの共有ポインタ、見つからなければnullptr</returns>
static std::shared_ptr<MeshRenderer> FindMeshRenderer(const std::shared_ptr<Entity>& e)
{
	for (const auto& c : e->GetComponents())
	{
		auto mr = std::dynamic_pointer_cast<MeshRenderer>(c);
		if (mr) return mr;
	}
	return nullptr;
}

/// <summary>
/// BLENDSTATE列挙型の値からインデックスを取得
/// </summary>
/// <param name="v">BLENDSTATEの値</param>
/// <returns>対応するインデックス、見つからなければ1（ALFA）</returns>
static int FindIndexBlend(BLENDSTATE v)
{
    const BLENDSTATE values[] = {
        BLENDSTATE_NONE, BLENDSTATE_ALFA, BLENDSTATE_ADD, BLENDSTATE_SUB, BLENDSTATE_MASK
    };
    for (int i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i)
        if (values[i] == v) return i;
    return 1; // ALFA
}

/// <summary>
/// SHADERMODE列挙型の値からインデックスを取得
/// </summary>
/// <param name="v">SHADERMODEの値</param>
/// <returns>対応するインデックス、見つからなければ0（COLOR）</returns>
static int FindIndexShader(SHADERMODE v)
{
    const SHADERMODE values[] = { SHADERMODE_COLOR, SHADERMODE_TEXTURE, SHADERMODE_LIGHTING };
    for (int i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i)
        if (values[i] == v) return i;
    return 0;
}

// ---------- 各コンポーネントの “描画本体” ----------

static void DrawTransform(const std::shared_ptr<Transform>& t)
{
    if (!t) return;

    ImGui::Separator();
    ImGui::Text("Transform");

    // Position
    {
        auto pos = t->GetPosition();
        float p[3] = { pos.x, pos.y, pos.z };
        if (ImGui::DragFloat3("Position", p, 0.1f))
            t->SetPosition({ p[0], p[1], p[2] });
    }

    // Rotation (Degrees)
    {
        auto rot = t->GetRotationDegrees();
        float r[3] = { rot.x, rot.y, rot.z };
        if (ImGui::DragFloat3("Rotation", r, 0.01f))
            t->SetRotationDegrees({ r[0], r[1], r[2] });
    }

    // Scale
    {
        auto scl = t->GetScale();
        float s[3] = { scl.x, scl.y, scl.z };
        if (ImGui::DragFloat3("Scale", s, 0.1f))
            t->SetScale({ s[0], s[1], s[2] });
    }
}

static void DrawMeshRenderer(const std::shared_ptr<MeshRenderer>& mr)
{
    if (!mr) return;

    ImGui::Separator();
    ImGui::Text("MeshRenderer");

    auto mat = mr->GetMaterial();
    if (!mat) return;

    ImGui::Text("Material");

    // ShaderMode
    {
        static const char* labels[] = { "Color", "Texture", "Lighting" };
        const SHADERMODE values[] = { SHADERMODE_COLOR, SHADERMODE_TEXTURE, SHADERMODE_LIGHTING };

        int idx = FindIndexShader(mat->GetShaderMode());
        if (ImGui::Combo("ShaderMode", &idx, labels, IM_ARRAYSIZE(labels)))
            mat->SetShader(values[idx]);
    }

    // BlendState
    {
        static const char* labels[] = { "None", "Alpha", "Add", "Sub", "Mask" };
        const BLENDSTATE values[] = {
            BLENDSTATE_NONE, BLENDSTATE_ALFA, BLENDSTATE_ADD, BLENDSTATE_SUB, BLENDSTATE_MASK
        };

        int idx = FindIndexBlend(mat->GetBlendState());
        if (ImGui::Combo("BlendState", &idx, labels, IM_ARRAYSIZE(labels)))
            mat->SetBlendState(values[idx]);
    }

    // BaseColor
    {
        auto c = mat->GetBaseColor();
        float col[4] = { c.x, c.y, c.z, c.w };
        if (ImGui::ColorEdit4("BaseColor", col, ImGuiColorEditFlags_Uint8))
            mat->SetBaseColor({ col[0], col[1], col[2], col[3] });
    }

    // Emissive
    {
        auto e = mat->GetEmissive();
        float col[3] = { e.x, e.y, e.z };
        if (ImGui::ColorEdit3("Emissive", col, ImGuiColorEditFlags_Uint8))
            mat->SetEmissive({ col[0], col[1], col[2] });
    }

    // Metallic / Roughness
    {
        float m = mat->GetMetallic();
        float r = mat->GetRoughness();

        if (ImGui::SliderFloat("Metallic", &m, 0.0f, 1.0f))
            mat->SetMetallic(m);

        if (ImGui::SliderFloat("Roughness", &r, 0.0f, 1.0f))
            mat->SetRoughness(r);
    }

    // Save Material
    static bool s_saveOk = true;
    if (ImGui::Button("Save Material"))
    {
        const auto& path = mr->GetMaterialPath();
        s_saveOk = !path.empty() ? mat->SaveToJson(path) : false;
    }
    ImGui::Text("MaterialPath: %s", mr->GetMaterialPath().c_str());
    ImGui::Text("Save Result: %s", s_saveOk ? "OK" : "FAILED");
}

// ---------- 登録テーブル（type名 → 描画） ----------

namespace
{
    using InspectorFn = std::function<void(const std::shared_ptr<Entity>&, const std::shared_ptr<Component>&)>;

    const std::unordered_map<std::string, InspectorFn>& GetInspectorTable()
    {
        static std::unordered_map<std::string, InspectorFn> table = {
            { "Transform", [](const std::shared_ptr<Entity>&, const std::shared_ptr<Component>& c)
                {
                    DrawTransform(std::dynamic_pointer_cast<Transform>(c));
                }
            },
            { "MeshRenderer", [](const std::shared_ptr<Entity>&, const std::shared_ptr<Component>& c)
                {
                    DrawMeshRenderer(std::dynamic_pointer_cast<MeshRenderer>(c));
                }
            },
        };
        return table;
    }
}

namespace Inspector
{
    void DrawComponentInspector(const std::shared_ptr<Entity>& entity,
        const std::shared_ptr<Component>& comp)
    {
        if (!comp) return;

        const std::string type = comp->GetTypeName();
        const auto& table = GetInspectorTable();

        auto it = table.find(type);
        if (it != table.end())
            it->second(entity, comp);
    }

    void DrawEntityInspector(const std::shared_ptr<Entity>& entity)
    {
        if (!entity)
        {
            ImGui::Text("未選択");
            return;
        }

        ImGui::Text("Name: %s", entity->GetName().c_str());

        // Component list
        //ImGui::Separator();
        //ImGui::Text("Components");
        //for (const auto& c : entity->GetComponents())
        //{
        //    if (!c) continue;
        //    ImGui::BulletText("%s", c->GetTypeName());
        //}

        // Draw each component inspector
        for (const auto& c : entity->GetComponents())
        {
            if (!c) continue;
            DrawComponentInspector(entity, c);
        }
    }
}
//=======================================================
// ファイル名	: ComponentSerdeRegistry.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/29
// 更新日		: 2026/01/29
// 詳細			: コンポーネントのシリアライズ/デシリアライズ関数群を登録および取得を行う
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "ComponentSerdeRegistry.h"	// コンポーネントシリアライズ/デシリアライズレジストリ
#include <vector>			// for std::vector
#include "Entity.h"			// エンティティクラス
#include "Component.h"		// コンポーネントクラス
#include "Transform.h"		// トランスフォームコンポーネントクラス
#include "MeshRenderer.h"	// メッシュレンダラーコンポーネントクラス
#include "Mesh.h"			// メッシュクラス
#include "Material.h"		// マテリアルクラス
#include "GameEngine.h"		// ゲームエンジンクラス
#include "Renderer.h"		// レンダラクラス
#include "Camera.h"		    // カメラクラス

//=======================================================
// 型エイリアス
//=======================================================
using json = nlohmann::json;

//=======================================================
// 名前空間
//=======================================================
namespace
{
	//--- 便利関数 ---

    // DirectX::XMFLOAT3 を配列 [x,y,z] に変換
    json ToJsonVec3(const DirectX::XMFLOAT3& v)
    {
        return json::array({ v.x, v.y, v.z });  // 配列として返す
    }

    // 配列 [x,y,z] を読む。失敗したら fallback を返す
    DirectX::XMFLOAT3 ReadVec3(const json& j, const char* key, const DirectX::XMFLOAT3& fallback)
    {
        // キーが存在しない場合や配列でない場合はフォールバックを返す
        if (!j.contains(key)) return fallback;  //フォールバック = デフォルト値

        // 配列を取得
        const auto& a = j.at(key);

        // 配列の形式チェック
        if (!a.is_array() || a.size() != 3) return fallback;

        // 各要素が数値でない場合はフォールバックを返す
        if (!a[0].is_number() || !a[1].is_number() || !a[2].is_number()) return fallback;

        // 正常に読み込めた場合は XMFLOAT3 を返す
        return DirectX::XMFLOAT3(
            a[0].get<float>(),
            a[1].get<float>(),
            a[2].get<float>()
        );
    }

	// --- Transform ---

    // Transformを保存する（Component -> JSON）
    void SaveTransform(const std::shared_ptr<Component>& c, json& outData)
    {
		auto t = std::dynamic_pointer_cast<Transform>(c);   // Transformにキャスト
        if (!t) return;

		outData["pos"] = ToJsonVec3(t->GetPosition());          // 位置
		outData["rot"] = ToJsonVec3(t->GetRotationDegrees());   // 回転（デグリー角）
		outData["scl"] = ToJsonVec3(t->GetScale());             // スケール
    }

    // Transformを復元する（JSON -> Entityに追加）
    void LoadTransform(Entity& e, const json& inData)
    {
		auto t = e.AddComponentNoAwake<Transform>();    // ロード中はAwakeしたくないので NoAwake

		t->SetPosition(ReadVec3(inData, "pos", t->GetPosition()));              // 位置
		t->SetRotationDegrees(ReadVec3(inData, "rot", t->GetRotationDegrees()));// 回転（デグリー角）
		t->SetScale(ReadVec3(inData, "scl", t->GetScale()));                    // スケール
    }

    // --- MeshRenderer ---

	// ビルトイン三角形メッシュを作成
    std::shared_ptr<Mesh> CreateBuiltinTriangleMesh()
    {
		// メッシュ作成
        auto mesh = std::make_shared<Mesh>();

		// 頂点データ
        std::vector<Vertex> vertices =
        {
            { {  0.0f,  0.5f, 0.0f }, {1, 0, 0, 1},{0,0}, { 0, 0, 1 } },
            { {  0.5f, -0.5f, 0.0f }, {0, 1, 0, 1},{0,1}, { 0, 0, 1 } },
            { { -0.5f, -0.5f, 0.0f }, {0, 0, 1, 1},{1,0}, { 0, 0, 1 } },
        };

		// インデックスデータ
        std::vector<UINT> indices = { 0, 1, 2 };

		// メッシュ初期化
        auto device = GameEngine::GetInstance().GetRenderer()->GetDevice();
        mesh->Init(device.Get(), vertices, indices);
		return mesh;    // メッシュを返す
    }

	// MeshRendererを復元する（JSON -> Entityに追加）
    void LoadMeshRenderer(Entity& e, const json& inData)
    {
        // ロード中は Awake させたくないので NoAwake
        auto mr = e.AddComponentNoAwake<MeshRenderer>();

		// メッシュIDの読み込み
        const std::string meshId = inData.value("mesh", std::string("Builtin/Triangle"));
		mr->SetMeshId(meshId);  // メッシュIDを設定

		// メッシュの設定
        if (meshId == "Builtin/Triangle")
            mr->SetMesh(CreateBuiltinTriangleMesh());

		// マテリアルパスの読み込み
        const std::string matPath = inData.value("material", std::string(""));
		mr->SetMaterialPath(matPath);   // マテリアルパスを設定
        
		// マテリアルの設定
        auto mat = std::make_shared<Material>();
		// マテリアルパスがあればロード
        if (!matPath.empty())
            mat->LoadFromJson(matPath);

        // マテリアルを設定
        mr->SetMaterial(mat);
    }

	// MeshRendererを保存する（Component -> JSON）
    void SaveMeshRenderer(const std::shared_ptr<Component>& c, json& outData)
    {
		auto mr = std::dynamic_pointer_cast<MeshRenderer>(c);   // MeshRendererにキャスト
        if (!mr) return;

		outData["mesh"] = mr->GetMeshId();              // メッシュID
        outData["material"] = mr->GetMaterialPath();    // マテリアルパス
    }

    // --- Camera ---
    
	// Cameraを保存する（Component -> JSON）
    void SaveCamera(const std::shared_ptr<Component>& c, json& outData)
    {
        auto cam = std::dynamic_pointer_cast<Camera>(c);
        if (!cam) return;

        outData["main"] = cam->IsMain();
        outData["fov"] = cam->GetFovDegrees();
        outData["near"] = cam->GetNearZ();
        outData["far"] = cam->GetFarZ();
    }

	// Cameraを復元する（JSON -> Entityに追加）
    void LoadCamera(Entity& e, const json& inData)
    {
        auto cam = e.AddComponentNoAwake<Camera>();
        cam->SetMain(inData.value("main", true));
        cam->SetFovDegrees(inData.value("fov", 60.0f));
        cam->SetNearZ(inData.value("near", 0.1f));
        cam->SetFarZ(inData.value("far", 100.0f));
    }

}

//=======================================================
// 名前空間：コンポーネントシリアライズ/デシリアライズレジストリ
//=======================================================
namespace ComponentSerdeRegistry
{
	// コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
    const std::unordered_map<std::string, ComponentSerde>& GetTable()
    {
		// 静的変数としてテーブルを定義
        static std::unordered_map<std::string, ComponentSerde> table =
        {
            { "Transform",    ComponentSerde{ SaveTransform,    LoadTransform } },
            { "MeshRenderer", ComponentSerde{ SaveMeshRenderer, LoadMeshRenderer } },
            { "Camera", ComponentSerde{ SaveCamera, LoadCamera } },
        };
        return table;
    }
}

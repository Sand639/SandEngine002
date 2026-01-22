//=======================================================
// ファイル名	: SceneSerializer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/17
// 更新日		: 2025/12/17
// 詳細			: Scene を JSON 形式で保存および読み込みを行う
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "SceneSerializer.h"    // シーンシリアライザクラス
#include <fstream>              // std::ifstream, std::ofstream
#include <iostream>             // std::cerr
#include <unordered_map>        // std::unordered_map
#include <functional>           // std::function
#include "External/json.hpp"    // nlohmann::json
#include "Scene.h"              // シーンクラス
#include "Entity.h"             // エンティティクラス
#include "Transform.h"          // トランスフォームコンポーネントクラス
#include "Debug.h"              // デバッグクラス

#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"

#include "GameEngine.h"
#include "Renderer.h"

//=======================================================
// 構造体定義
//=======================================================
/// コンポーネントのシリアライズ/デシリアライズ関数群
struct ComponentSerde
{
    std::function<void(const std::shared_ptr<Component>&, nlohmann::json& outData)> Save;
    std::function<void(Entity&, const nlohmann::json& inData)> Load;
};

// コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
static const std::unordered_map<std::string, ComponentSerde>& GetComponentSerdeTable();

//=======================================================
// 型エイリアス
//=======================================================
using json = nlohmann::json;

//=======================================================
// 名前空間
//=======================================================
namespace
{
	// 現在のシーンデータのバージョン
    constexpr int kSceneVersion = 1;

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
}


// Transformを保存する（Component -> JSON）
static void SaveTransform(const std::shared_ptr<Component>& c, json& outData)
{
    auto t = std::dynamic_pointer_cast<Transform>(c);
    if (!t) return; // 念のため

    outData["pos"] = ToJsonVec3(t->GetPosition());
    outData["rot"] = ToJsonVec3(t->GetRotationDegrees());
    outData["scl"] = ToJsonVec3(t->GetScale());
}

// Transformを復元する（JSON -> Entityに追加）
static void LoadTransform(Entity& e, const json& inData)
{
    // ロード中はAwakeしたくないので NoAwake
    auto t = e.AddComponentNoAwake<Transform>();

    t->SetPosition(ReadVec3(inData, "pos", t->GetPosition()));
    t->SetRotationDegrees(ReadVec3(inData, "rot", t->GetRotationDegrees()));
    t->SetScale(ReadVec3(inData, "scl", t->GetScale()));
}

static std::shared_ptr<Mesh> CreateBuiltinTriangleMesh()
{
    auto mesh = std::make_shared<Mesh>();

    std::vector<Vertex> vertices =
    {
        { {  0.0f,  0.5f, 0.0f }, {1, 0, 0, 1},{0,0}, { 0, 0, 1 } }, // 上（赤）
        {{  0.5f, -0.5f, 0.0f }, {0, 1, 0, 1},{0,1}, { 0, 0, 1 } }, // 右下（緑）
        { { -0.5f, -0.5f, 0.0f }, {0, 0, 1, 1},{1,0}, { 0, 0, 1 } }, // 左下（青）
    };

    std::vector<UINT> indices = { 0, 1, 2 };

    auto device = GameEngine::GetInstance().GetRenderer()->GetDevice();
    mesh->Init(device.Get(), vertices, indices);

    return mesh;
}



static void LoadMeshRenderer(Entity& e, const json& inData)
{
    // ロード中は Awake させたくないので NoAwake
    auto mr = e.AddComponentNoAwake<MeshRenderer>();

    // mesh（とりあえず Builtin/Triangle だけ対応）
    const std::string meshId = inData.value("mesh", std::string("Builtin/Triangle"));
    mr->SetMeshId(meshId);

    if (meshId == "Builtin/Triangle")
    {
        mr->SetMesh(CreateBuiltinTriangleMesh());
    }

    // material（パスがあればロード、無ければデフォルトMaterialを用意）
    auto mat = std::make_shared<Material>();

    const std::string matPath = inData.value("material", std::string(""));
    mr->SetMaterialPath(matPath);

    if (!matPath.empty())
    {
        mat->LoadFromJson(matPath);
    }

    mr->SetMaterial(mat);
}

static void SaveMeshRenderer(const std::shared_ptr<Component>& c, json& outData)
{
    auto mr = std::dynamic_pointer_cast<MeshRenderer>(c);
    if (!mr) return;

    outData["mesh"] = mr->GetMeshId();
    outData["material"] = mr->GetMaterialPath();
}

/// <summary>
/// コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
/// </summary>
/// <returns>コンポーネントのシリアライズ/デシリアライズ関数テーブル</returns>
static const std::unordered_map<std::string, ComponentSerde>& GetComponentSerdeTable()
{
        static std::unordered_map<std::string, ComponentSerde> table =
    {
        { "Transform", ComponentSerde{ SaveTransform, LoadTransform } },
        { "MeshRenderer", ComponentSerde{ SaveMeshRenderer, LoadMeshRenderer } }, // "MeshRenderer" を追加
    };

    return table;
}


/// <summary>
/// シーンを指定されたパスに保存
/// </summary>
/// <param name="scene">シーンオブジェクト</param>
/// <param name="path">保存先パス</param>
/// <returns>保存成功ならtrue、失敗ならfalse</returns>
bool SceneSerializer::Save(const Scene& scene, const std::string& path)
{
	// シーンデータを JSON オブジェクトに変換
    json root;
	root["version"] = kSceneVersion;        // シーンデータのバージョンを保存
	root["sceneName"] = scene.GetName();    // シーン名を保存
	root["entities"] = json::array();       // エンティティ配列を保存

	// シーン内の全エンティティをループ
    for (const auto& e : scene.GetEntities())
    {
		// エンティティが nullptr ならスキップ
        if (!e) continue;

		// エンティティ情報を保存
        json je;
		je["name"] = e->GetName();          // エンティティ名
		je["active"] = e->GetIsActive();    // エンティティの有効状態
		je["components"] = json::array();   // コンポーネント配列

		// コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
        auto& table = GetComponentSerdeTable();

		// エンティティ内の全コンポーネントをループ
        for (const auto& c : e->GetComponents())
        {
			// コンポーネントが nullptr ならスキップ
            if (!c) continue;

			// コンポーネントの型名を取得
            const std::string type = c->GetTypeName();
			// シリアライズ関数テーブルから対応する関数を探す
            auto it = table.find(type);
            if (it == table.end()) continue; // 未対応コンポーネントはスキップ

			// コンポーネント情報を保存
			json jc;    // コンポーネント JSON オブジェクト
			jc["type"] = type;  // コンポーネントの種類
			jc["data"] = json::object();  // コンポーネントのデータオブジェクト

			// シリアライズ関数を呼び出してデータを保存
            it->second.Save(c, jc["data"]);

			// コンポーネント情報をエンティティ情報に追加
            je["components"].push_back(jc);

        }

		// エンティティ情報をシーンデータに追加
        root["entities"].push_back(je);
    }

	// JSON データをファイルに保存
    std::ofstream out(path);
    if (!out.is_open())
    {
        Debug::LogError("SceneSerializer ファイルの書き込みができませんでした : " + path);
        return false;
    }

	// 整形して保存
    out << root.dump(4);
    return true;
}

/// <summary>
/// 指定されたパスからシーンを読み込み
/// </summary>
/// <param name="path">読み込み元パス</param>
/// <returns>読み込み成功ならシーンオブジェクト、失敗ならnullptr</returns>
std::shared_ptr<Scene> SceneSerializer::Load(const std::string& path)
{
    // 1. ファイルを開く
    std::ifstream f(path);
    if (!f)
    {
        Debug::LogError("SceneSerializerのJSONファイルが開けません : " + path);
        return nullptr;
    }

	// 2. JSON としてパース
    json root;
    try
    {
        root = json::parse(f);
    }
    catch (const std::exception& e)
    {
        Debug::LogError(std::string("SceneSerializer JSONのパースでエラー: " + std::string(e.what())));
        return nullptr;
    }
    
	// 3. バージョンチェック
    const int version = root.value("version", 0);
    if (version != kSceneVersion)
    {
        Debug::LogError(std::string("SceneSerializer JSONのバージョンが違います file = " + std::to_string(version)
            + " expected =" + std::to_string(kSceneVersion)));

		// TODO: バージョン違いの変換処理(マイグレーションを行う)
    }

	// 4. シーンデータを復元

    // 4-1 シーンの生成
    auto scene = std::make_shared<Scene>();

	//シーン名の設定
    scene->SetName(root.value("sceneName", std::string("New Scene")));

	// 4-2 エンティティの復元

	// Entityが存在しない場合は空シーンを返す
    if (!root.contains("entities") || !root["entities"].is_array())
        return scene;

    // コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
    auto& table = GetComponentSerdeTable();

	// JSONに含まれるEntity配列の数だけループ
    for (const auto& je : root["entities"])
    {
		// エンティティの生成
        auto e = scene->AddEntity<Entity>();
		// 名前の設定
        e->SetName(je.value("name", std::string("entity")));
        // アクティブ設定
        e->SetActive(je.value("active", true));

		// 4-3 コンポーネントの復元

		// components 配列が無い or 配列でない場合はスキップ
        if (!je.contains("components") || !je["components"].is_array())
            continue;

		// JSONに含まれるComponent配列の数だけループ
        for (const auto& jc : je["components"])
        {
			// type フィールドでコンポーネントの種類を判別
            const std::string type = jc.value("type", std::string(""));
			// データ部分を取得
            const auto& data = jc.contains("data") ? jc["data"] : jc;

            auto it = table.find(type);
            if (it != table.end())
            {
                it->second.Load(*e, data);
            }
        }
    }

	return scene;   // シーンを返す
}

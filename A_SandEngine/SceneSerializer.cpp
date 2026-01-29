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

#include "ComponentSerdeRegistry.h" // コンポーネントシリアライズ/デシリアライズレジストリ

//=======================================================
// 型エイリアス
//=======================================================
using json = nlohmann::json;

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
	root["version"] = ComponentSerdeRegistry::kSceneVersion;        // シーンデータのバージョンを保存
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
        const auto& table = ComponentSerdeRegistry::GetTable();

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
    if (version != ComponentSerdeRegistry::kSceneVersion)
    {
        Debug::LogError(std::string("SceneSerializer JSONのバージョンが違います file = " + std::to_string(version)
            + " expected =" + std::to_string(ComponentSerdeRegistry::kSceneVersion)));

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
    const auto& table = ComponentSerdeRegistry::GetTable();


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

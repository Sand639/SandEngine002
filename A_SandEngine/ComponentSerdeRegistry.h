//=======================================================
// ファイル名	: ComponentSerdeRegistry.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/29
// 更新日		: 2026/01/29
// 詳細			: コンポーネントのシリアライズ/デシリアライズ関数群を登録および取得を行う
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <functional>       // for std::function
#include <memory>           // for std::shared_ptr
#include <string>           // for std::string
#include <unordered_map>    // for std::unordered_map
#include "External/json.hpp"// for nlohmann::json

//=======================================================
// 前方宣言
//=======================================================
class Entity;
class Component;

//=======================================================
// 構造体宣言
//=======================================================

/// <summary>
/// コンポーネントのシリアライズ/デシリアライズ関数群
/// </summary>
/// <remarks>
/// コンポーネントのシリアライズ/デシリアライズ関数群を格納する構造体
/// </remarks>
struct ComponentSerde
{
	std::function<void(const std::shared_ptr<Component>&, nlohmann::json& outData)> Save;   // シリアライズ関数
	std::function<void(Entity&, const nlohmann::json& inData)> Load;    // デシリアライズ関数
};

namespace ComponentSerdeRegistry
{
	// 現在のシーンデータのバージョン
	constexpr int kSceneVersion = 1;

	// コンポーネントのシリアライズ/デシリアライズ関数テーブルを取得
    const std::unordered_map<std::string, ComponentSerde>& GetTable();
}

//=======================================================
// ファイル名	: ConfigLoader.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/10
// 更新日		: 2025/12/10
// 詳細			: 各種設定ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include <iostream>             // cerr 用
#include <fstream>              // ifstream 用
#include <string>               // std::string 用
#include "External\\json.hpp"   // nlohmann::json 用
#include "Config.h"             // Config struct 用
#include "Debug.h"              // デバッグ用

// nlohmann::json のエイリアス
using json = nlohmann::json;


/// <summary>
/// 設定ファイルを読み込む関数
/// </summary>
/// <param name="path">設定ファイルのパス</param>
/// <param name="outConfig">読み込んだ設定を格納する Config 構造体への参照</param>
/// <returns>読み込みに成功した場合は true、失敗した場合は false を返す</returns>
bool LoadConfig(const std::string& path, Config& outConfig)
{
    // 1. ファイルを開く
    std::ifstream f(path);
    if (!f)
    {
		Debug::LogError("ファイルが見つかりませんでした : " + path);
        return false;
    }

    try
    {
        // 2. JSON としてパース
        json data = json::parse(f);

        // 3. JSON から struct に値を詰める
        //    .value("キー", デフォルト値) を使うと、キーがなくても落ちない
        outConfig.fullscreen = data.value("fullscreen", false);
        outConfig.width = data.value("width", 1280);
        outConfig.height = data.value("height", 720);
        outConfig.title = data.value("title", std::string("Untitled"));

        return true;
    }
    catch (const std::exception& e)
    {
        Debug::LogError("JSONの形式が間違っています: " + std::string(e.what()));
        return false;
    }
}

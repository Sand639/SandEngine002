//=======================================================
// ファイル名	: InspectorRegistry.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2026/01/28
// 詳細		    : インスペクタ描画用のヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルードファイル
//=======================================================
#include <memory>   // std::shared_ptr
#include <string>   // std::string

//=======================================================
// 前方宣言
//=======================================================
class Entity;
class Component;

/// <summary>
/// インスペクタ描画用名前空間
/// </summary>
/// <remarks>
/// エンティティやコンポーネントのインスペクタ描画関数を提供します。
/// </remarks>
namespace Inspector
{
    // Component 1つ分のInspector描画
    void DrawComponentInspector(const std::shared_ptr<Entity>& entity,
        const std::shared_ptr<Component>& comp);

    // EntityのInspector全体（名前、コンポーネント一覧、各Inspector呼び出し）
    void DrawEntityInspector(const std::shared_ptr<Entity>& entity);
}

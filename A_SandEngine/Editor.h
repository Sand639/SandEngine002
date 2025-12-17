//=======================================================
// ファイル名	: Editor.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/17
// 詳細			: エディタクラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <memory>    // std::shared_ptr

//=======================================================
// 前方宣言
//=======================================================
class Scene;
class Entity;

/// <summary>
/// エディタクラス
/// </summary>
/// <remarks>
/// ゲームエンジンのエディタ機能を管理するクラス
/// </remarks>
class Editor
{
private:
	bool m_doneUninit = false;   // 終了処理が完了したかのフラグ
	bool m_visible = true;      // エディタの表示状態
	bool m_showHierarchy = true;// ヒエラルキーウィンドウの表示状態
	bool m_showInspector = true;// インスペクターウィンドウの表示状態

	//最後に選択したエンティティ
    std::shared_ptr<Entity> m_selectedEntity;


private:
    
	// --- ウィンドウ描画関数 ---

    void DrawHierarchy(const std::shared_ptr<Scene>& _scene);
    void DrawInspector();

public:

	// --- ライフサイクル関数 ---

    void Init();
    void Update();
    void Draw(const std::shared_ptr<Scene>& _scene);
    void Uninit();

    bool IsVisible() const { return m_visible; }
    void SetVisible(bool v) { m_visible = v; }


};
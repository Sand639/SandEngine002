//=======================================================
// ファイル名	: GameEngine.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: ゲームエンジン本体のヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <Windows.h>    // Windows API
#include <memory>       // スマートポインタを使う
#include "Window.h"     // ウィンドウクラス

/// <summary>
/// ゲームエンジン本体
/// </summary>
/// <remarks>
/// ウィンドウの初期化、メインループ、終了処理を行うクラス
/// </remarks>
class GameEngine
{
private:
    // ウィンドウ管理クラス
    std::unique_ptr<Window> m_window;

    // ターゲットFPS（デフォルト60）
	int m_fps = 60;

public:

    // デストラクタ
    ~GameEngine() { Uninit(); }

    // 初期化処理関数
    bool Init(HINSTANCE hInstance, const wchar_t* title, int width, int height);

    // メインループ処理関数
    int Run();

    // 終了処理関数
    void Uninit();

};
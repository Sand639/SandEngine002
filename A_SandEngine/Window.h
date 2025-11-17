//=======================================================
// ファイル名	: Window.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
    bool Init(HINSTANCE hInstance, const wchar_t* title, int width, int height, int nCmdShow);
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <windows.h>	//ウィンドウズAPI

/// <summary>
/// ウィンドウクラス
/// </summary>
/// <remarks>
/// ウィンドウの初期化、メッセージループ処理、破棄を行うクラス
/// </remarks>
class Window
{
private:
    // ウィンドウの識別子
    HWND m_hWnd = nullptr;

    // ウィンドウプロシージャ(コールバック関数)
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    // ウィンドウを作成する関数
    bool Init(HINSTANCE hInstance, const wchar_t* title, int width, int height);
    
    // ウィンドウ破棄関数
    void Uninit();

    // メッセージループ処理関数
    bool ProcessMessages();

    // ゲッター：ウィンドウハンドルを取得
    HWND GetHandle() const { return m_hWnd; }

};
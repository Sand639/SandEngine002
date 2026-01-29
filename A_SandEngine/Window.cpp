//=======================================================
// ファイル名	: Window.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: ウィンドウを作成・管理するクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Window.h"		// ウィンドウクラス
#include "MessageBox.h" // メッセージボックスラッパークラス

#include <windows.h>


//=======================================================
// グローバル定数定義
//=======================================================
constexpr const wchar_t* WINDOW_CLASS_NAME = L"SandEngineWindowClass"; // ウィンドウクラス名
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// ImGuiのウィンドウプロシージャハンドラ

/// <summary>
/// ウィンドウプロシージャ関数(コールバック関数)
/// </summary>
/// <param name="hWnd">ウィンドウの識別子</param>
/// <param name="message">メッセージの種類</param>
/// <param name="wParam">メッセージの追加情報1</param>
/// <param name="lParam">メッセージの追加情報2</param>
/// <returns>メッセージ処理結果</returns>
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGuiのウィンドウプロシージャハンドラを呼び出し
	if (LRESULT r = ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return r;

	// メッセージの種類に応じて処理を分岐
	switch (message)
	{
	case WM_CLOSE:	// ウィンドウを閉じる
		PostQuitMessage(0);	// 終了メッセージをキューに送る
		return 0;	// 終了する
		break;
	default:
		// その他のメッセージはデフォルトの処理に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

/// <summary>
/// ウィンドウの初期化関数
/// </summary>
/// <param name="hInstance">アプリケーションの識別子</param>
/// <param name="title">ウィンドウの名前</param>
/// <param name="width">ウィンドウの横のサイズ</param>
/// <param name="height">ウィンドウの縦のサイズ</param>
/// <returns>初期化が成功下かの判定</returns>
bool Window::Init(HINSTANCE hInstance, const wchar_t* title, int width, int height)
{
	// 1. ウィンドウクラスの設定
	WNDCLASSEXW wcex{};									// ウィンドウクラス構造体の宣言と初期化
	wcex.cbSize = sizeof(WNDCLASSEXW);					// 構造体のサイズを設定
	wcex.style = CS_HREDRAW | CS_VREDRAW;				// ウィンドウサイズ変更時に再描画
	wcex.lpfnWndProc = WndProc;							// コールバック関数として自身の静的メソッドを設定
	wcex.hInstance = hInstance;							//アプリケーションの識別子を設定
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);    // デフォルトのアイコンを設定
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);      // デフォルトのカーソルを設定
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// ウィンドウの背景色を設定
	wcex.lpszClassName = WINDOW_CLASS_NAME;				// ウィンドウクラス名を設定

	// ウィンドウクラスの登録
	if (!RegisterClassExW(&wcex))
	{
		MessageBoxWrapper::errorMessage("ウィンドウクラスの登録に失敗しました");
		return false;	// 登録失敗
	}

	// ウィンドウサイズを調整
	RECT rc{ 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int winW = rc.right - rc.left;
	int winH = rc.bottom - rc.top;

	// 2. ウィンドウの作成
	m_hWnd = CreateWindowExW(
		0,					//拡張ウィンドウスタイル
		WINDOW_CLASS_NAME,	//ウィンドウクラス名
		title,				//ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,//ウィンドウスタイル
		CW_USEDEFAULT,		// 初期X座標
		CW_USEDEFAULT,		// 初期Y座標
		winW,				// ウィンドウの幅
		winH,				// ウィンドウの高さ
		nullptr,			// 親ウィンドウハンドル
		nullptr,			// メニューハンドル
		hInstance,			// アプリケーションの識別子
		nullptr				// 追加パラメータ
	);

	// ウィンドウ作成失敗時の処理
	if (!m_hWnd)
	{
		// 作成失敗時
		MessageBoxWrapper::errorMessage("ウィンドウクラスの作成に失敗しました");
		return false;
	}

	// ウィンドウを表示し、OSに描画してもらう
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);	//即描画する

	return true; // 成功
}

/// <summary>
/// メッセージ処理関数 (ゲームループの核)
/// </summary>
/// <returns>継続するかの判定</returns>
bool Window::ProcessMessages()
{
	// メッセージ構造体の宣言
	MSG msg{};

	// PeekMessageはメッセージがあれば処理し、なくてもすぐに制御を返す
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//キーボードメッセージをWM_CHARに変換
		DispatchMessage(&msg);	//メッセージをウィンドウプロシージャに送る

		// WM_QUITメッセージが来たら、エンジンに終了を知らせるために false を返す
		if (msg.message == WM_QUIT)
		{
			return false;	//終了する
		}
	}
	return true;
}

/// <summary>
/// ウィンドウ終了処理関数
/// </summary>
void Window::Uninit()
{
	// ウィンドウが存在する場合にのみ破棄処理を行う
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd); // ウィンドウを破棄
		m_hWnd = nullptr;      // ハンドルをクリア
	}
}
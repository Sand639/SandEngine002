//=======================================================
// ファイル名	: Main.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: メイン関数が書かれているファイル
//=======================================================

//=======================================================
// ライブラリのリンク設定
//=======================================================
#pragma comment(lib, "d3d11.lib")			// DirectX11ライブラリ
#pragma comment (lib, "d3dcompiler.lib")	// シェーダーコンパイラライブラリ
#pragma	comment (lib, "winmm.lib")			// マルチメディアAPIライブラリ0
#pragma	comment (lib, "dxguid.lib")			// DirectX GUIDライブラリ
#pragma	comment (lib, "dinput8.lib")		// DirectInputライブラリ

//=======================================================
// インクルード
//=======================================================
#include "Main.h"		// メイン関数のヘッダファイル
#include <windows.h>	//ウィンドウズAPI
	if (!engine->Init(hInstance, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, nCmdShow))
#include "GameEngine.h" // ゲームエンジンクラス

//=======================================================
// グローバル定数定義
//=======================================================
const wchar_t* WINDOW_TITLE = L"SandEngine-DX11"; // ウィンドウのタイトル

/// <summary>
/// メイン関数
/// </summary>
/// <param name="hInstance">アプリケーションのインスタンスハンドル（識別子）</param>
/// <param name="hPrevInstance">以前のインスタンスハンドル</param>
/// <param name="lpCmdLine">コマンドライン引数（ANSI文字列）</param>
/// <param name="nCmdShow">アプリケーションのウィンドウをどのように表示するか（最大化、最小化など）を指定するフラグ</param>
/// <returns>プログラムの終了コードをOSに返します。通常は正常終了で 0 を返します。</returns>
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//乱数初期化
	srand((UINT)timeGetTime());

	// ゲームエンジンのスマートポインタを作成
	std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();

	// ゲームエンジンの初期化
	if (!engine->Init(hInstance, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		// 初期化失敗時は終了
		return 1;
	}

	// メインループの実行
	return engine->Run();
}

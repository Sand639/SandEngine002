//=======================================================
#endif
#endif
// 制作日		: 2025/11/02
// 詳細			: メッセージボックスを表示する時に使うラッパークラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "MessageBox.h"	// メッセージボックスラッパークラス
#include <windows.h>	//ウィンドウズAPI
#include <string>       // std::wstring

/// <summary>
/// エラーメッセージを表示する時に使用するラッパー関数
/// </summary>
/// <param name="message">エラー内容</param>
/// <param name="title">メッセージボックスのタイトル</param>
void MessageBoxWrapper::errorMessage(const char* message, const char* title)
{
	//デバッグビルド時の処理
#if defined(_DEBUG)
	// デバッガにメッセージを出力
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
	// メッセージボックスを表示
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
#endif
}

/// <summary>
/// HRESULTのエラーメッセージを表示する時に使用するラッパー関数
/// </summary>
/// <param name="hr">HRESULT値</param>
/// <param name="title">メッセージボックスのタイトル</param>
void MessageBoxWrapper::ShowHRErrorMessage(HRESULT hr, const std::wstring& title) {
#if defined(_DEBUG)
	// デバッガにHRESULTコードを出力
	OutputDebugStringW((L"HRESULT エラーコード: 0x" + std::to_wstring(hr) + L"\n").c_str());

	// エラーコードからメッセージテキストを取得
	LPWSTR errorText = nullptr;

	// FormatMessageWを使用してエラーメッセージを取得
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,    // フラグ
		nullptr,    // メッセージソース
		hr,         // メッセージID (HRESULTコード)
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // 言語ID
		(LPWSTR)&errorText, // バッファへのポインタ
		0,                  // バッファサイズ (自動割り当てのため0)
		nullptr             // 引数リスト
	);

	// メッセージの準備
	std::wstring message;

	// 取得したメッセージテキストがあれば使用、なければ不明なエラーコードを表示
	if (errorText) {
		message = errorText;	// メッセージテキストを設定
		LocalFree(errorText);	// バッファを解放
	}
	else {
		message = L"不明なエラーコード: 0x" + std::to_wstring(hr);
	}

	// メッセージボックスを表示
	MessageBoxW(nullptr, message.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#endif
}

#pragma once
//=======================================================
// ファイル名	: messageBox.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/02
// 詳細			: メッセージボックスを表示する時に使うラッパー関数を管理するクラスのヘッダファイル
//=======================================================
#pragma once

//=======================================================
// インクルード
//=======================================================
#include <windows.h>	//ウィンドウズAPI
#include <string>       // std::wstring

/// <summary>
/// メッセージボックスを表示する時に使うラッパークラス
/// </summary>
/// <remarks>
/// Windows API の MessageBox 関数をラップし、使いやすくするためのクラス
/// </remarks>
class MessageBoxWrapper
{
public:

	static void errorMessage(const char* message, const char* title = "エラーが発生しました");

	static void ShowHRErrorMessage(HRESULT hr, const std::wstring& title = L"HRESULT エラー");

};
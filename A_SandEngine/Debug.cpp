//=======================================================
// ファイル名	: Debug.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: デバッグ関連の関数をまとめたクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Debug.h"		// デバッグクラス
#include "MessageBox.h" // メッセージボックスラッパークラス
#include <iostream>     // std::cout

/// <summary>
/// HRESULTのチェック関数
/// </summary>
/// <param name="hr">チェックするHRESULT値</param>
/// <returns>成功したかの判定</returns>
bool Debug::CheckHR(HRESULT hr, const std::wstring& title)
{
	// エラー時の処理
	if (FAILED(hr)) {
		MessageBoxWrapper::ShowHRErrorMessage(hr, title);
		return false;	// 失敗
	}
	return true;	// 成功

}


void Debug::LogInternal(LogLevel level, const std::string& message)
{
#ifdef _DEBUG
    const char* levelStr = "";
    switch (level)
    {
    case LogLevel::Info:    levelStr = "[Info] ";    break;
    case LogLevel::Warning: levelStr = "[Warning] "; break;
    case LogLevel::Error:   levelStr = "[Error] ";   break;
    }

    std::string out = std::string(levelStr) + message + "\n";

    // コンソールに出力
    std::cout << out;

    // Visual Studio の Output ウィンドウにも出す（デバッグ時に便利）
    OutputDebugStringA(out.c_str());
#endif
}

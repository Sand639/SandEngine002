//=======================================================
// ファイル名	: Debug.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: デバッグ関連の関数をまとめたクラスのヘッダーファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <Windows.h>	// Windows API
#include <string>       // std::wstring
#include <cstdarg>
#include <vector>
#include <format> // std::vformat, std::make_format_args 用


//=======================================================
// マクロ定義
//=======================================================
#define DEBUG_LOG(fmt, ...)        DebugLogFormat(LogLevel::Info,    fmt, ##__VA_ARGS__)
#define DEBUG_LOG_WARN(fmt, ...)   DebugLogFormat(LogLevel::Warning, fmt, ##__VA_ARGS__)
#define DEBUG_LOG_ERROR(fmt, ...)  DebugLogFormat(LogLevel::Error,   fmt, ##__VA_ARGS__)


// ログレベル列挙型
enum class LogLevel
{
	Info,
	Warning,
	Error,
};

/// <summary>
/// デバッグ関連の関数をまとめたクラス
/// </summary>
/// <remarks>
/// デバッグビルド時にのみ有効なデバッグ用関数を提供するクラス
/// </remarks>
class Debug
{
public:	//　--- メッセージボックス系の関数 ---

	// HRESULTのチェック関数
	static bool CheckHR(HRESULT hr, const std::wstring& title = L"HRESULT エラー");

public: // --- コンソールログ系の関数 ---
	
	// 情報ログ関数
    template<typename... Args>
    static void Log(const std::string& fmt, Args&&... args)
    {
#ifdef _DEBUG
        std::string msg = std::vformat(fmt, std::make_format_args(args...));
        LogInternal(LogLevel::Info, msg);
#endif
    }


	// 警告ログ関数
    template<typename... Args>
    static void LogWarning(const std::string& fmt, Args&&... args)
    {
#ifdef _DEBUG
        std::string msg = std::vformat(fmt, std::make_format_args(args...));
        LogInternal(LogLevel::Warning, msg);
#endif
    }

	// エラーログ関数
    template<typename... Args>
    static void LogError(const std::string& fmt, Args&&... args)
    {
#ifdef _DEBUG
        std::string msg = std::vformat(fmt, std::make_format_args(args...));
        LogInternal(LogLevel::Error, msg);
#endif
    }

private:
	static void LogInternal(LogLevel level, const std::string& message);

};
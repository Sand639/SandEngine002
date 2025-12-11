//=======================================================
// ファイル名	: StringConvert.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/10
// 更新日		: 2025/12/10
// 詳細			: 文字列変換ユーティリティのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <string>   // std::string

namespace StringConvert
{
    // UTF-8 (std::string) -> UTF-16 (std::wstring)
    std::wstring ToWideString(const std::string& str);

    // UTF-16 (std::wstring) -> UTF-8 (std::string)
    std::string ToUtf8String(const std::wstring& wstr);
}
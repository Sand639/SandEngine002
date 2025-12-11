//=======================================================
// ファイル名	: StringConvert.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/10
// 更新日		: 2025/12/10
// 詳細			: 文字列変換ユーティリティの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "StringConvert.h"
#include <Windows.h>

namespace StringConvert
{

	// UTF-8 (std::string) -> UTF-16 (std::wstring)
	std::wstring ToWideString(const std::string& str)
	{
		// 入力文字列が空の場合、空のwstringを返す
		if (str.empty()) return std::wstring();

		// 1. 変換後の文字数を計算する
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(),
			static_cast<int>(str.size()), nullptr, 0);

		// 2. バッファ（入れ物）を用意する
		std::wstring wstr(size_needed, 0);

		// 3. 実際に変換する
		MultiByteToWideChar(CP_UTF8, 0, str.data(),
			static_cast<int>(str.size()), &wstr[0], size_needed);

		return wstr;	// 変換後のwstringを返す
	}

	std::string ToUtf8String(const std::wstring& wstr)
	{
		// 入力文字列が空の場合、空のstringを返す
		if (wstr.empty()) return std::string();

		// 1. 変換後のバイト数を計算する
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
			nullptr, 0, nullptr, nullptr);

		// 2. バッファ（入れ物）を用意する
		std::string str(size_needed, 0);

		// 3. 実際に変換する
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
			&str[0], size_needed, nullptr, nullptr);

		return str;
	}
}

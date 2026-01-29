//=======================================================
// ファイル名	: Input.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/17
// 詳細			: 入力管理クラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <Windows.h> 

/// <summary>
/// 入力管理クラス
/// </summary>
/// <remarks>
/// キーボード入力を管理するクラス
/// </remarks>
class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static bool m_Initialized;

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

	static bool GetAnyKeyPress();
	static bool GetAnyKeyTrigger();
};

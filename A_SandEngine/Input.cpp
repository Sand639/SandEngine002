//=======================================================
// ファイル名	: Input.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/17
// 詳細			: 入力管理クラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Input.h"

//=======================================================
// 静的メンバ変数の定義
//=======================================================
BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
bool Input::m_Initialized;

void Input::Init()
{

	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
	m_Initialized = false;
}

void Input::Uninit()
{


}

void Input::Update()
{

	if (!m_Initialized)
	{
		GetKeyboardState(m_KeyState);
		memcpy(m_OldKeyState, m_KeyState, 256);
		m_Initialized = true;
	}
	else
	{
		memcpy(m_OldKeyState, m_KeyState, 256);
		GetKeyboardState(m_KeyState);
	}

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetAnyKeyPress()
{
	for (int i = 8; i <= 222; ++i)
	{
		if (GetKeyPress(i)) return true;
	}
	return false;
}

bool Input::GetAnyKeyTrigger()
{
	for (int i = 8; i <= 222; ++i)
		if (GetKeyTrigger(i)) return true;
	return false;
}

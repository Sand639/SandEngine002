//=======================================================
// ファイル名	: FrameTimer.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: フレームタイマーのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <windows.h>	//ウィンドウズAPI

/// <summary>
/// フレームタイマークラス
/// </summary>
/// <remarks>
/// フレームごとの時間管理を行うクラス
/// </remarks>
class FrameTimer
{
private:
	// 目標FPS（デフォルト60）
	int m_fps = 60;

	// 時間管理用変数
	DWORD m_dwExecLastTime = 0;	// 前回のゲーム更新を実行した時間
	DWORD m_dwFPSLastTime = 0;	// FPSカウント用の基準時間
	DWORD m_dwCurrentTime = 0;	// 現在の時間
	DWORD m_dwFrameCount = 0;	// 今の1秒間で描画したフレーム数

	// 経過時間（秒）
	float m_deltaTimeSec = 0.0f;

public:
	// コンストラクタ
	FrameTimer(int _fps = 60);
	~FrameTimer();

public:
	//1フレーム進めていいタイミングでtrueを返す関数
	bool Tick();

	//セッター

	// 目標FPSの設定
	void SetFPS(int fps) { m_fps = fps; }

	//ゲッター

	// 前のフレームからの経過時間（秒）を取得
	float GetDeltaTime() const { return m_deltaTimeSec; }

	// 現在のFPSを知りたくなったとき用に
	DWORD GetFrameCount() const { return m_dwFrameCount; }
};
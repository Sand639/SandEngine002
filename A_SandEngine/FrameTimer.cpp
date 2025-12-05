//=======================================================
// ファイル名	: FrameTimer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: フレームタイマーの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "FrameTimer.h" // フレームタイマークラス
#include <mmsystem.h>   // マルチメディアAPI（timeGetTime関数用）

/// <summary>
/// コンストラクタ
/// </summary>
FrameTimer::FrameTimer(int _fps) : m_fps(_fps)
{
	// 高精度タイマーの設定
    timeBeginPeriod(1);

	m_dwExecLastTime = timeGetTime();   //現在の時間を取得
	m_dwFPSLastTime = m_dwExecLastTime; //FPS計測用の時間を初期化
	m_dwCurrentTime = 0;	//現在の時間を初期化
	m_dwFrameCount = 0.0f;		//フレームカウントを初期化
}

/// <summary>
/// デストラクタ
/// </summary>
FrameTimer::~FrameTimer()
{
	// 高精度タイマーの終了
    timeEndPeriod(1);
}

/// <summary>
/// 1フレーム進めていいタイミングでtrueを返す関数
/// </summary>
/// <returns>1フレーム進めていいタイミングかの判定</returns>
bool FrameTimer::Tick()
{
	//現在の時間を取得
	m_dwCurrentTime = timeGetTime();

	// 前回の実行時間からの経過時間（ミリ秒）
	DWORD elapsedMs = m_dwCurrentTime - m_dwExecLastTime;

	// 経過時間が1フレーム分に達していなければ、まだ進まない
	const DWORD frameSpanMs = 1000 / m_fps;
	if (elapsedMs < frameSpanMs)
	{
		return false; // まだ進まない
	}

	// DeltaTime（秒）を更新
	m_deltaTimeSec = static_cast<float>(elapsedMs) / 1000.0f;

	// 前回の実行時間を更新
	m_dwExecLastTime = m_dwCurrentTime;

	// FPSカウント
	++m_dwFrameCount;

	// 1秒経過したらFPSカウントをリセットする
	DWORD fpsElapsedMs = m_dwCurrentTime - m_dwFPSLastTime;
	if (fpsElapsedMs >= 1000)
	{
		m_dwFPSLastTime = m_dwCurrentTime;	// FPS計測用の時間を更新
		m_dwFrameCount = 0;					// フレームカウントをリセット
	}

	return true;	// 1フレーム進む
}

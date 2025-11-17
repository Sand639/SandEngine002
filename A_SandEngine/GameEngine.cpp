//=======================================================
#include <algorithm>     // std::max
bool GameEngine::Init(HINSTANCE hInstance, const wchar_t* title, int width, int height, int nCmdShow)
    if (!m_window->Init(hInstance, title, width, height, nCmdShow))
	// vpϐ
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	// ^C}\̐ݒ
	timeBeginPeriod(1);
	// 
	dwExecLastTime = dwFPSLastTime = timeGetTime();// ݎ
	dwCurrentTime = 0;
	dwFrameCount = 0;

	const int targetFPS = std::max(m_fps, 1);
	const DWORD frameSpan = std::max<DWORD>(1, static_cast<DWORD>(1000.0f / static_cast<float>(targetFPS)));
	while (true)
	{
		if (!m_window->ProcessMessages())
		{
			break;	// WM_QUIT MI
		dwCurrentTime = timeGetTime();
		if ((dwCurrentTime - dwFPSLastTime) >= 1000)
		{
			dwFPSLastTime = dwCurrentTime;
			dwFrameCount = 0;
		}

		const DWORD elapsed = dwCurrentTime - dwExecLastTime;
		if (elapsed < frameSpan)
		{
			const DWORD sleepTime = frameSpan - elapsed;
			if (sleepTime > 0)
			{
				::Sleep(sleepTime);
			}
			else
			{
				::Sleep(0);
			}
			continue;
		}
		dwExecLastTime = dwCurrentTime;
		// 3. Q[̍XVƕ`
		// Update();
		// Draw();
		++dwFrameCount;
	}
	timeEndPeriod(1);
	return 0;

int GameEngine::Run()
{
    // 時間管理用変数
    DWORD	dwExecLastTime;
    DWORD	dwFPSLastTime;
    DWORD	dwCurrentTime;
    DWORD	dwFrameCount;

    //タイマーの分解能を設定
    timeBeginPeriod(1);

    //フレームレート計測初期化
    dwExecLastTime = dwFPSLastTime = timeGetTime();//現在のタイマー値
    dwCurrentTime = dwFrameCount = 0;

	// メインループ
    while (true)
    {

		// 1. ウィンドウメッセージ処理
        if (!m_window->ProcessMessages())
        {
            break; // WM_QUIT が来たらループ終了
		}

		// 2. 時間更新
        dwCurrentTime = timeGetTime();  //現在のタイマー値を取得

        // FPS カウント（1秒経過ごとにリセット）
        if ((dwCurrentTime - dwFPSLastTime) >= 1000)//1秒経過したか
        {
            dwFPSLastTime = dwCurrentTime;	//現在のタイマー値を保存
            dwFrameCount = 0;				//フレームカウントをクリア
        }

        // 固定FPS制御（m_fps を元に更新タイミングを決定）
        const DWORD frameSpan = static_cast<DWORD>(1000.0f / m_fps);
        if ((dwCurrentTime - dwExecLastTime) >= frameSpan)
        {
            dwExecLastTime = dwCurrentTime;	//現在の時間を保存

            // 3. ゲーム内処理
			// Update();
			// Draw();


            ++dwFrameCount;	//フレームカウントを進める
        }
    }

    // タイマー精度を元に戻す
    timeEndPeriod(1);

    return 0;
}


// 終了処理
void GameEngine::Uninit()
{


	// ウィンドウの終了処理
    if (m_window)   // ウィンドウが存在する場合
    {
        m_window->Uninit();   // ウィンドウの破棄
        m_window.reset();       // スマートポインタの参照を解除し、メモリを解放
    }
}
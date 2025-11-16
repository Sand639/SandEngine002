//=======================================================
// ファイル名	: GameEngine.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: ゲームエンジンクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "GameEngine.h" // ゲームエンジンクラス
#include "Main.h"       // メイン関数ヘッダファイル(画面高さ、幅の定義)
#include <mmsystem.h>   // マルチメディアAPI（timeGetTime関数用）

/// <summary>
/// 初期化処理関数
/// </summary>
/// <param name="hInstance">アプリケーションの識別子</param>
/// <param name="title">ウィンドウの名前</param>
/// <param name="width">ウィンドウの横のサイズ</param>
/// <param name="height">ウィンドウの縦のサイズ</param>
/// <returns>初期化が成功したかの判定</returns>
bool GameEngine::Init(HINSTANCE hInstance, const wchar_t* title, int width, int height)
{

    // 1. Windowクラスの生成
    m_window = std::make_unique<Window>();

    // Windowの初期化。失敗したらUninitを呼び出し、falseを返す
    if (!m_window->Init(hInstance, title, width, height))
    {
        Uninit();     // 終了処理
        return false;   // 初期化失敗
    }

    return true;
}

/// <summary>
/// メインループ処理関数
/// </summary>
/// <returns></returns>
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
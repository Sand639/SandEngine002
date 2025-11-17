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

	// 2. フレームタイマーの生成と初期化
	m_frameTimer = std::make_unique<FrameTimer>(m_fps);

    return true;
}

/// <summary>
/// メインループ処理関数
/// </summary>
/// <returns></returns>
int GameEngine::Run()
{
	// メインループ
    while (true)
    {

		// ウィンドウメッセージ処理
        if (!m_window->ProcessMessages())
        {
            break; // WM_QUIT が来たらループ終了
		}

		// Update();
		// LateUpdate();
		
		// 固定FPS制御
        if (m_frameTimer->Tick())
        {
            float deltaTime = m_frameTimer->GetDeltaTime();

            // ここでゲーム更新＆描画
            // Update(deltaTime);
            // LateUpdate(deltaTime);
            // FixedUpdate();
            // Draw();
        }
    }

    return 0;
}


// 終了処理
void GameEngine::Uninit()
{
    // フレームタイマーの解放
    if (m_frameTimer)
    {
        m_frameTimer.reset();
    }

	// ウィンドウの終了処理
    if (m_window)   // ウィンドウが存在する場合
    {
        m_window->Uninit();   // ウィンドウの破棄
        m_window.reset();       // スマートポインタの参照を解除し、メモリを解放
    }
}
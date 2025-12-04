//=======================================================
// ファイル名	: GameEngine.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/18
// 詳細			: ゲームエンジンクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "GameEngine.h" // ゲームエンジンクラス
#include "Main.h"       // メイン関数ヘッダファイル(画面高さ、幅の定義)
#include "Renderer.h"   // レンダラークラス
#include "TestScene.h"  // テストシーンクラス
#include "DebugConsole.h" // デバッグコンソールクラス

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
		Uninit();       // 終了処理
		return false;   // 初期化失敗
	}

	// 2. Rendererクラスの生成
	m_renderer = std::make_unique<Renderer>();

	// Rendererの初期化。失敗したらUninitを呼び出し、falseを返す
	if (!m_renderer->Init(m_window->GetHandle(), width, height))
	{
		Uninit();       // 終了処理
		return false;   // 初期化失敗
	}

	// 3. フレームタイマーの生成と初期化
	m_frameTimer = std::make_unique<FrameTimer>(m_fps);

	// 4. シーンの初期化
	m_currentScene = nullptr;

	//TODO: 前回のシーンをロードする処理を追加
	// LoadScene();

	//TODO: 前回のシーンがなければデフォルトシーンを作成する処理を追加
	// if (!m_currentScene) CreateDefaultScene();

	//今はとりあえずTestSceneをセットしておく
	m_currentScene = std::make_unique<TestScene>();
	m_currentScene->Init();

	return true;
}

/// <summary>
/// メインループ処理関数
/// </summary>
/// <returns></returns>
int GameEngine::Run()
{
	Awake();
	Start();

	// メインループ
	while (true)
	{
		// ウィンドウメッセージ処理
		if (!m_window->ProcessMessages())
		{
			break; // WM_QUIT が来たらループ終了
		}

		// 固定FPS制御
		if (m_frameTimer->Tick())
		{
			// ここでゲーム更新＆描画
			Update();
			LateUpdate();
			FixedUpdate();
			Draw();
			EndOfFrame();

			// シーンチェンジ処理
			ChangeScene();
		}
	}

	Uninit(); // 終了処理

	return 0;
}

// 起動処理
void GameEngine::Awake()
{
	DebugConsole::Open(); // デバッグコンソールを開く

	if (m_currentScene)
		m_currentScene->Awake();
}

// 開始処理
void GameEngine::Start()
{
	if (m_currentScene)
		m_currentScene->Start();
}

void GameEngine::Update()
{
	if (m_currentScene)
		m_currentScene->Update();
}

void GameEngine::LateUpdate()
{
	if (m_currentScene)
		m_currentScene->LateUpdate();
}

void GameEngine::FixedUpdate()
{
	if (m_currentScene)
		m_currentScene->FixedUpdate();
}

void GameEngine::Draw()
{
	// 描画開始
	m_renderer->Begin();
	// ここで描画処理

	// 3D 用の行列をセット
	m_renderer->SetWorldViewProjection3D();

	if (m_currentScene)
		m_currentScene->Draw();

	// 描画終了
	m_renderer->End();
}

void GameEngine::EndOfFrame()
{
	if (m_currentScene)
		m_currentScene->EndOfFrame();
}

// 終了処理
void GameEngine::Uninit()
{

	if (m_currentScene)
	{
		m_currentScene->Uninit(); // シーンの終了処理
	}

	// フレームタイマーの解放
	if (m_frameTimer)
	{
		m_frameTimer.reset();
	}

	// レンダラーの終了処理
	if (m_renderer)
	{
		m_renderer->Uninit();   // レンダラーの破棄
		m_renderer.reset();     // スマートポインタの参照を解除し、メモリを解放
	}

	DebugConsole::Close(); // デバッグコンソールを閉じる

	// ウィンドウの終了処理
	if (m_window)   // ウィンドウが存在する場合
	{
		m_window->Uninit();     // ウィンドウの破棄
		m_window.reset();       // スマートポインタの参照を解除し、メモリを解放
	}
}

void GameEngine::ChangeScene()
{
	if (m_nextScene)
	{
		if (m_currentScene)
		{
			m_currentScene->Uninit(); // 現在のシーンが存在する場合、終了処理を行う
		}
		m_currentScene = m_nextScene; // 新しいシーンを設定
		if (m_currentScene)
		{
			m_currentScene->Init();		// 新しいシーンが存在する場合、初期化を行う
			m_currentScene->Awake();	// 新しいシーンのAwakeを呼び出す
			m_currentScene->Start();	// 新しいシーンのStartを呼び出す
		}

		m_nextScene.reset(); // 次のシーンをクリア
	}

}

//=======================================================
// ファイル名	: GameEngine.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2026/01/29
// 詳細			: ゲームエンジンクラスの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "GameEngine.h"		// ゲームエンジンクラス
#include "Main.h"			// メイン関数ヘッダファイル(画面高さ、幅の定義)
#include "Renderer.h"		// レンダラークラス
#include "TestScene.h"		// テストシーンクラス
#include "DebugConsole.h"	// デバッグコンソールクラス

#include "Debug.h"			// デバッグクラス
#include "Config.h"			// 設定構造体
#include "ConfigLoader.h"	// 設定ファイルローダークラス
#include "StringConvert.h"	// 文字列変換クラス

#include "Input.h"			// 入力管理クラス
#include "SceneSerializer.h"// シーンシリアライザクラス

#include "Camera.h"			// カメラクラス


/// <summary>
/// 初期化処理関数
/// </summary>
/// <param name="hInstance">アプリケーションの識別子</param>
/// <param name="title">ウィンドウの名前</param>
/// <param name="width">ウィンドウの横のサイズ</param>
/// <param name="height">ウィンドウの縦のサイズ</param>
/// <returns>初期化が成功したかの判定</returns>
bool GameEngine::Init(HINSTANCE hInstance)
{
	// 0. 設定ファイルの読み込み
	Config config{};

	if (!LoadConfig("config.json", config))
	{
		Debug::LogError("設定ファイルの読み込みに失敗しました: config.json");
		return false;
	}

	m_config = config;

	// ウィンドウタイトルをワイド文字列に変換
	std::wstring titleWide = StringConvert::ToWideString(config.title);


	// 1. Windowクラスの生成
	m_window = std::make_unique<Window>();

	// Windowの初期化。失敗したらUninitを呼び出し、falseを返す
	if (!m_window->Init(hInstance, titleWide.c_str(), config.width, config.height))
	{
		Uninit();       // 終了処理
		return false;   // 初期化失敗
	}

	// 2. Rendererクラスの生成
	m_renderer = std::make_unique<Renderer>();

	// Rendererの初期化。失敗したらUninitを呼び出し、falseを返す
	if (!m_renderer->Init(m_window->GetHandle(), config.width, config.height))
	{
		Uninit();       // 終了処理
		return false;   // 初期化失敗
	}

	// 3. Editorクラスの初期化
	m_editor = std::make_unique<Editor>();
	m_editor->Init();


	// 4. フレームタイマーの生成と初期化
	m_frameTimer = std::make_unique<FrameTimer>(m_fps);

	// 5. 入力管理クラスの初期化
	Input::Init(); // 入力管理クラスの初期化

	// 6. シーンの初期化
	m_currentScene = nullptr;

	// シーンの読み込み
	const std::string scenePath = "Assets/Scenes/TestScene.json";

	// シーンのロード
	m_currentScene = SceneSerializer::Load(scenePath);
	if (!m_currentScene)
	{
		// 無ければデフォルト生成
		m_currentScene = std::make_shared<TestScene>();
	}

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
	// 入力管理クラスの更新
	Input::Update();

	// エディタの更新
	if (m_editor) m_editor->Update();

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

	if (m_currentScene)
	{
		// --- カメラの適用 ---

// シーン内のエンティティを走査してカメラコンポーネントを探す
		auto cam = Camera::FindMainCamera(m_currentScene);

		// メインカメラが存在する場合、そのビュー・プロジェクション行列を設定
		if (cam)
		{
			const float aspect = m_renderer->GetAspect();

			m_renderer->SetViewMatrix(cam->GetViewMatrix());
			m_renderer->SetProjectionMatrix(cam->GetProjectionMatrix(aspect));
		}
		else	// カメラが存在しない場合
		{
			// 3D描画用ワールド・ビュー・プロジェクション行列設定
			m_renderer->SetWorldViewProjection3D();
		}

		// シーンの描画
		m_currentScene->Draw();

	}
	

	//エディタの描画
	if (m_editor)
		m_editor->Draw(m_currentScene);

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

	if (m_editor)
	{
		m_editor->Uninit(); // エディタの終了処理
		m_editor.reset();
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

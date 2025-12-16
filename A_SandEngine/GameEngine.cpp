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

#include "Debug.h"			// デバッグクラス
#include "Config.h"			// 設定構造体
#include "ConfigLoader.h"	// 設定ファイルローダークラス
#include "StringConvert.h"	// 文字列変換クラス

#include "ImGuiLayer.h"		// ImGuiレイヤークラス
#include "imgui.h"			// ImGui本体

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

	// 3. ImGuiレイヤーの初期化
	ImGuiLayer::Init(
		m_window->GetHandle(),
		m_renderer->GetDevice().Get(),
		m_renderer->GetDeviceContext().Get()
	);

	// 4. フレームタイマーの生成と初期化
	m_frameTimer = std::make_unique<FrameTimer>(m_fps);

	// 5. シーンの初期化
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
	// F1トグル（押した瞬間だけ反応）
	const bool f1Down = (GetAsyncKeyState(VK_F1) & 0x8000) != 0;
	if (f1Down && !m_prevF1Down)
		m_showHierarchy = !m_showHierarchy;
	m_prevF1Down = f1Down;

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

	// ImGui描画開始
	ImGuiLayer::BeginOfDraw();

	// ImGuiデモウィンドウ表示
	if(m_showImGuiDemoWindow)
		ImGui::ShowDemoWindow();

if (m_showHierarchy && m_currentScene)
{
    ImGui::Begin(IMGUI_U8("ヒエラルキー"), &m_showHierarchy);

    const auto& entities = m_currentScene->GetEntities();
    for (const auto& e : entities)
    {
		bool selected = (m_selectedEntity == e);
		if (ImGui::Selectable(e->GetName().c_str(), selected))
			m_selectedEntity = e;
    }

	ImGui::End();

	ImGui::Begin(IMGUI_U8("インスペクタ"));
	if (m_selectedEntity)
	{
		ImGui::Text("Name: %s", m_selectedEntity->GetName().c_str());
		// Transformがあるならここで編集（DragFloat3など）
	}
	else
	{
		ImGui::Text(IMGUI_U8("未選択"));
	}
	

    ImGui::End();
}

	// 3D描画用ワールド・ビュー・プロジェクション行列設定
	m_renderer->SetWorldViewProjection3D();

	if (m_currentScene)
		m_currentScene->Draw();

	// ImGui描画終了
	ImGuiLayer::EndOfDraw();

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

	ImGuiLayer::Uninit();

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

//=======================================================
// ファイル名	: GameEngine.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 詳細			: ゲームエンジン本体のヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <Windows.h>    // Windows API
#include <memory>       // スマートポインタを使う
#include "Window.h"     // ウィンドウクラス
#include "FrameTimer.h" // フレームタイマークラス
#include "Renderer.h"   // レンダラクラス
#include "scene.h"      // シーンクラス


/// <summary>
/// ゲームエンジン本体
/// </summary>
/// <remarks>
/// ウィンドウの初期化、メインループ、終了処理を行うクラス
/// </remarks>
class GameEngine
{
private:
    // ウィンドウ管理クラス
    std::unique_ptr<Window> m_window;

	// フレームタイマー
	std::unique_ptr<FrameTimer> m_frameTimer;
	int m_fps = 60; // 目標FPS

    // レンダラークラス
    std::unique_ptr<Renderer> m_renderer;

	// 現在のシーン
	std::shared_ptr<Scene> m_currentScene;

	//次のシーン
	std::shared_ptr<Scene> m_nextScene;

private:

    // ライフサイクル

	void Awake();
	void Start();

	void Update();
	void LateUpdate();
    void FixedUpdate();

	void Draw();

	void EndOfFrame();

    void Uninit();



	void ChangeScene();

public:

    // デストラクタ
    ~GameEngine() { Uninit(); }

    // 初期化処理関数
    bool Init(HINSTANCE hInstance, const wchar_t* title, int width, int height);

    // メインループ処理関数
    int Run();

	// シングルトン取得関数
    static GameEngine& GetInstance()
    {
        static GameEngine instance;
        return instance;
	}

	// セッター

	// 目標FPSの設定
	void SetFPS(int fps)
	{
		m_fps = fps;
		if (m_frameTimer)
			m_frameTimer->SetFPS(fps);
	}

	// シーンの設定
	template <typename T>
	void SetNextScene() { m_nextScene = std::make_shared<T>(); }

	void SetNextScene(std::shared_ptr<Scene> scene) { m_nextScene = scene; }

    //ゲッター

	// 現在のシーンを取得
	std::shared_ptr<Scene> GetCurrentScene() const { return m_currentScene; }

	// レンダラーを取得
	Renderer* GetRenderer() const { return m_renderer.get(); }

	// ウィンドウを取得
	Window* GetWindow() const { return m_window.get(); }

	// 前のフレームからの経過時間（秒）を取得
	float GetDeltaTime() const
	{
		if (m_frameTimer)
			return m_frameTimer->GetDeltaTime();
		return 0.0f;
	}


};
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
#include "Renderer.h"


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

    // ライフサイクル

	void Awake();
	void Start();

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
    void FixedUpdate();

	void Draw();
    
    void Uninit();


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

	Renderer* GetRenderer() const { return m_renderer.get(); }

};
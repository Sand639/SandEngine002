//=======================================================
// ファイル名	: ImGuiLayer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/16
// 詳細			: ImGuiレイヤーの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include <windows.h>    	// HWND
#include <d3d11.h>       	// ID3D11Device, ID3D11DeviceContext

// ImGui関連ヘッダー
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

//=======================================================
// 名前空間定義
//=======================================================

// ImGuiレイヤー
namespace ImGuiLayer
{
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="hwnd">ウィンドウハンドル</param>
    /// <param name="device">D3D11デバイス</param>
    /// <param name="context">D3D11デバイスコンテキスト</param>
    void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
    {
		IMGUI_CHECKVERSION();       // バージョンチェック
		ImGui::CreateContext();     // ImGuiコンテキストの作成
		ImGui::StyleColorsDark();   // ダークテーマの適用

		ImGui_ImplWin32_Init(hwnd);  // Win32バックエンドの初期化
		ImGui_ImplDX11_Init(device, context);   // DX11バックエンドの初期化

        // 日本語フォント追加（Windows標準）
		ImGuiIO& io = ImGui::GetIO();   // ImGuiIO参照取得
		ImFontConfig cfg{};             // フォント設定構造体
		cfg.FontNo = 0;                 // フォントコレクション内のフォントインデックス指定
		cfg.OversampleH = 3;            // 横方向のオーバーサンプリング設定
		cfg.OversampleV = 1;            // 縦方向のオーバーサンプリング設定

		// メイリオフォントを18ptで追加
        auto* font = io.Fonts->AddFontFromFileTTF(
            "C:\\Windows\\Fonts\\meiryo.ttc",   // メイリオフォントを読み込むパス
			18.0f,  						    // フォントサイズ         
			&cfg,   					        // フォント設定構造体へのポインタ
			io.Fonts->GetGlyphRangesJapanese()  // 日本語グリフ範囲指定
        );

        IM_ASSERT(font && "Failed to load Japanese font (meiryo.ttc)");
        io.FontDefault = font;
    }

    /// <summary>
    /// 終了処理
    /// </summary>
    void Uninit()
    {
		ImGui_ImplDX11_Shutdown();  // DX11バックエンドのシャットダウン
		ImGui_ImplWin32_Shutdown(); // Win32バックエンドのシャットダウン
		ImGui::DestroyContext();    // ImGuiコンテキストの破棄
    }

    /// <summary>
    /// 1フレーム開始（毎フレーム）
    /// </summary>
    /// <remarks>この関数は、ImGuiの新しいフレームを開始します。</remarks>
    void BeginOfDraw()
    {
		ImGui_ImplDX11_NewFrame();  // DX11バックエンドの新しいフレーム開始
		ImGui_ImplWin32_NewFrame(); // Win32バックエンドの新しいフレーム開始
		ImGui::NewFrame();          // ImGuiの新しいフレーム開始
    }

    /// <summary>
    /// 1フレーム終了（毎フレーム）
    /// </summary>
    /// <remarks>この関数は、ImGuiの描画を実行し、描画データをデバイスに送信します。</remarks>
    void EndOfDraw()
    {
		ImGui::Render();    // ImGuiの描画を実行
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());    // 描画データをDX11デバイスに送信
    }

}
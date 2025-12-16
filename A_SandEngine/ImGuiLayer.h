//=======================================================
// ファイル名	: ImGuiLayer.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/16
// // 詳細			: ImGuiレイヤーのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <windows.h>
#include <d3d11.h>

//=======================================================
// マクロ定義
//=======================================================
#if __cpp_char8_t
    #define IMGUI_U8(str) reinterpret_cast<const char*>(u8##str)
#else
    #define IMGUI_U8(str) u8##str
#endif

// ImGuiレイヤー
namespace ImGuiLayer
{
    //プロトタイプ宣言

    void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Uninit();
    void BeginOfDraw();
    void EndOfDraw();
}
//=======================================================
// ファイル名	: DebugConsole.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/27
// 更新日		: 2025/11/27
// 詳細			: デバッグコンソールの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "DebugConsole.h"
#include <Windows.h>
#include <cstdio>


/// <summary>
/// デバッグコンソールを開く関数
/// </summary>
/// <remarks>
/// デバッグビルド時にのみコンソールウィンドウを開き、標準出力と標準エラー出力をリダイレクトします。
/// </remarks>
namespace DebugConsole
{
    void Open()
    {
#ifdef _DEBUG
        AllocConsole();

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);  // std::cout 用
        freopen_s(&fp, "CONOUT$", "w", stderr);  // std::cerr 用

        // 必要なら stdin も
        // freopen_s(&fp, "CONIN$", "r", stdin);

        printf("Debug console initialized.\n");
#endif
    }

    void Close()
    {
#ifdef _DEBUG
        FreeConsole();
#endif
    }
}

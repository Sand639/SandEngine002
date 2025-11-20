//=======================================================
// ファイル名	: Renderer.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: レンダラークラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <Windows.h>	// Windows API
#include <d3d11.h>		// DirectX11(グラフィクスAPI)
#include <wrl/client.h>	// Microsoft::WRL::ComPtr
#include <DirectXMath.h>// DirectXMath (数学ライブラリ)
#include "main.h"		// デフォルトのスクリーンサイズ定義

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//ブレンドステート定義
enum BLENDSTATE
{
    BLENDSTATE_NONE = 0,	//ブレンドしない
    BLENDSTATE_ALFA,		//普通のαブレンド
    BLENDSTATE_ADD,			//加算合成
    BLENDSTATE_SUB,			//減算合成
    BLENDSTATE_MASK,		//マスク合成
    BLENDSTATE_MAX
};

class Renderer
{
private:
    ComPtr<ID3D11Device> m_device;                           // GPUを抽象化したデバイス
    ComPtr<ID3D11DeviceContext> m_deviceContext;             // 描画コマンドを実行するコンテキスト
    ComPtr<IDXGISwapChain> m_swapChain;                      // ウィンドウに描画するためのバッファ管理
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;       // 描画の出力先ビュー
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;       // 深度ステンシルビュー

	ComPtr<ID3D11Buffer> m_worldBuffer;                      // 定数バッファ (ワールド行列用)
	ComPtr<ID3D11Buffer> m_viewBuffer;                       // 定数バッファ (ビュー行列用)
	ComPtr<ID3D11Buffer> m_projectionBuffer;    				// 定数バッファ (射影行列用)


	ComPtr< ID3D11DepthStencilState> m_depthStateEnable;     // 深度ステンシルステート 有効
	ComPtr< ID3D11DepthStencilState> m_depthStateDisable;    // 深度ステンシルステート 無効

	ComPtr<ID3D11BlendState> m_blendState[BLENDSTATE_MAX];   // ブレンドステート配列

public:

	// コンストラクタ
	Renderer() = default;
	// デストラクタ
	virtual ~Renderer() = default;

	bool Init(HWND hWnd, int width = WINDOW_WIDTH, int height = WINDOW_HEIGHT);	// 初期化
	void Uninit();																// 終了処理
	void Begin();																// 描画開始
	void End();																	// 描画終了

	//セッター

	// ブレンドステート設定
	void SetBlendState(BLENDSTATE _blend);

	// 深度ステンシルステート設定
	void SetDepthEnable(bool _enable);

	// 2D描画用ワールド・ビュー・プロジェクション行列設定
	void SetWorldViewProjection2D();
	// 3D描画用ワールド・ビュー・プロジェクション行列設定
	void SetWorldViewProjection3D();
	void ResetWorldViewProjection3D();

	// 各種行列設定
	void SetWorldMatrix(DirectX::XMMATRIX _worldMatrix);
	void SetViewMatrix(DirectX::XMMATRIX _viewMatrix);
	void SetProjectionMatrix(DirectX::XMMATRIX _projectionMatrix);

	// ゲッター
	
	// デバイス取得
	ComPtr<ID3D11Device> GetDevice() { return m_device; }
	// デバイスコンテキスト取得
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_deviceContext; }


};
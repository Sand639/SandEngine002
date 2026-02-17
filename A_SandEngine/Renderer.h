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

//=======================================================
// 名前空間
//=======================================================
template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//=======================================================
// 前方宣言
//=======================================================
struct CB_Material;

//=======================================================
// 列挙体定義
//=======================================================

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
	int m_width = 0;		// 画面の幅
	int m_height = 0;	// 画面の高さ

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;						// GPUを抽象化したデバイス
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;		// 描画コマンドを実行するコンテキスト
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;					// ウィンドウに描画するためのバッファ管理
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;	// 描画の出力先ビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// 深度ステンシルビュー

	ComPtr<ID3D11Buffer> m_worldBuffer;						// 定数バッファ (ワールド行列用)
	ComPtr<ID3D11Buffer> m_viewBuffer;						// 定数バッファ (ビュー行列用)
	ComPtr<ID3D11Buffer> m_projectionBuffer;    			// 定数バッファ (射影行列用)
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_materialBuffer;	// 定数バッファ (マテリアル用)


	ComPtr< ID3D11DepthStencilState> m_depthStateEnable;	// 深度ステンシルステート 有効
	ComPtr< ID3D11DepthStencilState> m_depthStateDisable;	// 深度ステンシルステート 無効

	ComPtr<ID3D11BlendState> m_blendState[BLENDSTATE_MAX];	 // ブレンドステート配列

public:

	// コンストラクタ
	Renderer() = default;
	// デストラクタ
	virtual ~Renderer() = default;

	bool Init(HWND hWnd, int width = WINDOW_WIDTH, int height = WINDOW_HEIGHT);	// 初期化
	void Uninit();																// 終了処理
	void Begin();																// 描画開始
	void End();																	// 描画終了

	//シェーダーの作成関数
	bool CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	bool CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	// --- セッター / ゲッター ---

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

	// マテリアル用定数バッファ取得
	void SetMaterial(const CB_Material& data);
	ID3D11Buffer* GetMaterialBuffer() const { return m_materialBuffer.Get(); }
	
	// デバイス取得
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_device; }
	// デバイスコンテキスト取得
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_deviceContext; }

	// 画面サイズ取得
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	float GetAspect() const { return (m_height > 0) ? (float)m_width / (float)m_height : 1.0f; }

};
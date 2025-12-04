//=======================================================
// ファイル名	: Renderer.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/17
// 更新日		: 2025/11/17
// 詳細			: レンダラークラスのソースファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Renderer.h"		// レンダラークラス
#include "MessageBox.h"		// メッセージボックスラッパークラス
#include "Debug.h"			// デバッグクラス
#include <d3dcompiler.h>	// シェーダーコンパイラ
#include <io.h>				// ファイル操作	

//=======================================================
// 名前空間
//=======================================================
using namespace DirectX;

/// <summary>
/// 初期化関数
/// </summary>
/// <param name="hWnd">ウィンドウの識別子</param>
/// <param name="width">ウィンドウの横幅</param>
/// <param name="height">ウィンドウの縦幅</param>
/// <returns>初期化が成功したかの判定</returns>
bool Renderer::Init(HWND hWnd, int width, int height)
{
	// 戻り値用変数
	HRESULT hr = S_OK;
	
	// スワップチェーンの設定構造体
	DXGI_SWAP_CHAIN_DESC sd{};

	// 1. デバイス、スワップチェーン作成
	{
		// スワップチェーンの構造体設定
		ZeroMemory(&sd, sizeof(sd));                        // 構造体の初期化
		sd.BufferCount = 2;                                 // バックバッファの数 (ダブルバッファリングなら2)
		sd.BufferDesc.Width = width;                        // バックバッファの幅
		sd.BufferDesc.Height = height;                      // バックバッファの高さ
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // カラーフォーマット
		sd.BufferDesc.RefreshRate.Numerator = 60;           // リフレッシュレートの分子 60Hz
		sd.BufferDesc.RefreshRate.Denominator = 1;          // リフレッシュレートの分母
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 描画ターゲットとして利用
		sd.OutputWindow = hWnd;                             // 描画先のウィンドウ識別子
		sd.SampleDesc.Count = 1;                            // マルチサンプリングなし
		sd.Windowed = TRUE;                                 // ウィンドウモード
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;      // スワップ効果

		// デバイス、デバイスコンテキスト、スワップチェーンの作成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,                        // グラフィックアダプタの指定 (nullptrでデフォルト)
			D3D_DRIVER_TYPE_HARDWARE,       // ハードウェア駆動を指定
			nullptr,                        // ソフトウェアデバイスは使用しない
			0,                              // 作成オプション (0でデフォルト)
			nullptr,                        // 使用する機能レベルの配列 (nullptrでデフォルト)
			0,                              // 機能レベルの数
			D3D11_SDK_VERSION,              // SDKバージョン
			&sd,                            // スワップチェーンの設定
			m_swapChain.GetAddressOf(),     // 生成されたスワップチェーン
			m_device.GetAddressOf(),        // 生成されたデバイス
			nullptr,                        // 特性レベル
			m_deviceContext.GetAddressOf()  // 生成されたデバイスコンテキスト
		);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"スワップチェーンの作成に失敗しました")) return false;

	}

	// 2. レンダーターゲットビューの作成
	{
		ComPtr<ID3D11Texture2D> backBuffer{}; // バックバッファ用テクスチャ
		// スワップチェーンからバックバッファを取得
		hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		Debug::CheckHR(hr);    // 初期化失敗をチェック

		// バックバッファからレンダーターゲットビューを作成
		hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

		// エラーチェック
		if (!Debug::CheckHR(hr, L"レンダーターゲットビューの作成に失敗しました")) return false;
	}


	// 3. デプスステンシルバッファ作成
	{
		// デプスステンシルバッファ作成
		ID3D11Texture2D* depthStencil{};									// デプスステンシル用テクスチャ
		D3D11_TEXTURE2D_DESC textureDesc{};									// テクスチャの設定構造体
		textureDesc.Width = sd.BufferDesc.Width;							// テクスチャの幅
		textureDesc.Height = sd.BufferDesc.Height = height;					// テクスチャの高さ
		textureDesc.MipLevels = 1;											// ミップマップレベル数	
		textureDesc.ArraySize = 1;											// 配列サイズ
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;					// デプスステンシルフォーマット
		textureDesc.SampleDesc = sd.SampleDesc;								// サンプリング設定
		textureDesc.Usage = D3D11_USAGE_DEFAULT;							// 標準的な使用法
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;					// デプスステンシルバッファとして使用
		textureDesc.CPUAccessFlags = 0;										// CPUアクセスなし
		textureDesc.MiscFlags = 0;											// その他の設定なし
		hr = m_device->CreateTexture2D(&textureDesc, NULL, &depthStencil);	// テクスチャの作成

		// エラーチェック
		if (!Debug::CheckHR(hr, L"デプスステンシルバッファの作成に失敗しました")) return false;

		// デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};										// デプスステンシルビューの設定構造体
		depthStencilViewDesc.Format = textureDesc.Format;											// フォーマット
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;							// 2Dテクスチャとして使用
		depthStencilViewDesc.Flags = 0;																// その他の設定なし
		hr = m_device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, m_depthStencilView.GetAddressOf());	// デプスステンシルビューの作成
																// テクスチャ解放
		// エラーチェック
		if (!Debug::CheckHR(hr, L"デプスステンシルビューの作成に失敗しました")) return false;

		depthStencil->Release();
		depthStencil = nullptr;																// スマートポインタに移動
	}

	// 4. レンダーターゲットとデプスステンシルビューをOMに設定
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());


	// 5. ビューポートの設定
	{
		// ビューポートの設定
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;    // ビューポートの幅
		viewport.Height = (float)height;  // ビューポートの高さ
		viewport.MinDepth = 0.0f;         // 最小深度
		viewport.MaxDepth = 1.0f;         // 最大深度
		viewport.TopLeftX = 0;            // ビューポートの左上X座標
		viewport.TopLeftY = 0;            // ビューポートの左上Y座標

		// パイプラインにビューポートを設定
		m_deviceContext->RSSetViewports(1, &viewport);
	}

	// 6. ラスタライザステート設定
	{
		// ラスタライザステートの設定
		D3D11_RASTERIZER_DESC rasterizerDesc{};		// ラスタライザステートの設定構造体
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;	// 塗りつぶしモード
		rasterizerDesc.CullMode = D3D11_CULL_BACK;	// 背面カリング
		rasterizerDesc.DepthClipEnable = TRUE;		// デプスクリッピング有効化
		rasterizerDesc.MultisampleEnable = FALSE;	// マルチサンプリング無効化

		// ラスタライザステートの作成
		ID3D11RasterizerState* rs;
		hr = m_device->CreateRasterizerState(&rasterizerDesc, &rs);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"ラスタライザステートの作成に失敗しました")) return false;

		// パイプラインにラスタライザステートを設定
		m_deviceContext->RSSetState(rs);

		rs->Release();
	}

	// 7. ブレンドステートの作成
	{
		// ブレンドステートの設定構造体
		D3D11_BLEND_DESC blendDesc{};                     // ブレンドステートの設定構造体
		blendDesc.AlphaToCoverageEnable = FALSE;          // アルファ・トゥ・カバレッジ無効化
		blendDesc.IndependentBlendEnable = FALSE;         // 独立ブレンド無効化
		
		// レンダーターゲットのブレンド設定
		D3D11_RENDER_TARGET_BLEND_DESC& rtbd = blendDesc.RenderTarget[0];
		rtbd.BlendEnable = TRUE;                          // ブレンド有効化
		rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;            // ソースブレンドファクター
		rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;       // デスティネーションブレンドファクター
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;                // ブレンド演算
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;             // アルファソースブレンドファクター
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;           // アルファデスティネーションブレンドファクター
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;           // アルファブレンド演算
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 書き込みマスク
		
		//各種ブレンドステートの作成

		//ブレンド無効
		blendDesc.RenderTarget[0].BlendEnable = FALSE;	//ブレンド無効
		hr = m_device->CreateBlendState(&blendDesc, &m_blendState[BLENDSTATE_NONE]);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"ブレンドステート(BLENDSTATE_NONE)の作成に失敗しました")) return false;


		//αブレンド
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		hr = m_device->CreateBlendState(&blendDesc, &m_blendState[BLENDSTATE_ALFA]);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"ブレンドステート(BLENDSTATE_ALFA)の作成に失敗しました")) return false;


		//加算合成
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		hr = m_device->CreateBlendState(&blendDesc, &m_blendState[BLENDSTATE_ADD]);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"ブレンドステート(BLENDSTATE_ADD)の作成に失敗しました")) return false;


		//減算合成
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		hr = m_device->CreateBlendState(&blendDesc, &m_blendState[BLENDSTATE_SUB]);

		// エラーチェック
		if (!Debug::CheckHR(hr, L"ブレンドステート(BLENDSTATE_SUB)の作成に失敗しました")) return false;


		//初期のブレンドステートをαブレンドに設定
		SetBlendState(BLENDSTATE_ALFA);
	}

	// 8. デプスステンシルステートの作成
	{
		// デプスステンシルステートの設定構造体
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};		// デプスステンシルステートの設定構造体
		depthStencilDesc.DepthEnable = TRUE;				// デプスバッファ有効化
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 深度書き込みマスク
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // 深度比較関数

		// デプスステンシルステートの作成
		ComPtr<ID3D11DepthStencilState> depthStateEnable{};
		hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStateEnable);	//深度有効ステート
		
		// エラーチェック
		if (!Debug::CheckHR(hr, L"デプスステンシルステート(深度有効)の作成に失敗しました")) return false;
		
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStateDisable);//深度無効ステート

		// エラーチェック
		if (!Debug::CheckHR(hr, L"デプスステンシルステート(深度無効)の作成に失敗しました")) return false;

		// パイプラインにデプスステンシルステートを設定
		m_deviceContext->OMSetDepthStencilState(m_depthStateEnable.Get(), 0);
	}

	// 9. サンプラーステートの作成
	{
		// サンプラーステートの設定構造体
		D3D11_SAMPLER_DESC samplerDesc{};						// サンプラーステートの設定構造体
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;			// フィルタリング方法
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// U座標のアドレスタイプ
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// V座標のアドレスタイプ
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// W座標のアドレスタイプ
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// 比較関数
		samplerDesc.MaxAnisotropy = 4;							// 最大異方性フィルタリング(設定で変動できるようにした方がいい)
		samplerDesc.MinLOD = 0.0f;								// 最小LOD
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;					// 最大LOD
		
		// サンプラーステートの作成
		ComPtr<ID3D11SamplerState> samplerState{};
		hr = m_device->CreateSamplerState(&samplerDesc, &samplerState);
		
		// エラーチェック
		if (!Debug::CheckHR(hr, L"サンプラーステートの作成に失敗しました")) return false;
		
		// パイプラインにサンプラーステートを設定
		m_deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	}

	// 10. 定数バッファの作成
	{
		// 定数バッファの設定構造体
		D3D11_BUFFER_DESC bd{};                     // バッファの設定構造体
		bd.Usage = D3D11_USAGE_DEFAULT;             // 標準的な使用法
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // 定数バッファとして使用
		bd.CPUAccessFlags = 0;                      // CPUアクセスなし
		bd.MiscFlags = 0;							// その他の設定なし
		bd.StructureByteStride = sizeof(float);		// ワールド行列用定数バッファの作成

		bd.ByteWidth = sizeof(XMFLOAT4X4); // バッファのサイズ

		// ワールド行列用定数バッファの作成
		hr = m_device->CreateBuffer(&bd, NULL, m_worldBuffer.GetAddressOf());
		
		// エラーチェック
		if (!Debug::CheckHR(hr, L"定数バッファ(ワールド行列)の作成に失敗しました")) return false;
	
		// パイプラインに定数バッファを設定
		ID3D11Buffer* worldCB = m_worldBuffer.Get();
		m_deviceContext->VSSetConstantBuffers(0, 1, &worldCB);

	
		// ビュー行列用定数バッファの作成
		hr = m_device->CreateBuffer(&bd, NULL, m_viewBuffer.GetAddressOf());

		// エラーチェック
		if (!Debug::CheckHR(hr, L"定数バッファ(ビュー行列)(の作成に失敗しました")) return false;

		// パイプラインに定数バッファを設定
		ID3D11Buffer* viewCB = m_viewBuffer.Get();
		m_deviceContext->VSSetConstantBuffers(1, 1, &viewCB);

		// 射影行列用定数バッファの作成
		hr = m_device->CreateBuffer(&bd, NULL, m_projectionBuffer.GetAddressOf());

		// エラーチェック
		if (!Debug::CheckHR(hr, L"定数バッファ(射影行列)の作成に失敗しました")) return false;

		// パイプラインに定数バッファを設定
		ID3D11Buffer* projectionCB = m_projectionBuffer.Get();
		m_deviceContext->VSSetConstantBuffers(2, 1, &projectionCB);

	}

	return true;    // 初期化成功
}

/// <summary>
/// 終了処理
/// </summary>
void Renderer::Uninit()
{

	if (m_swapChain)
	{
		// ウィンドウモードに戻す
		m_swapChain->SetFullscreenState(FALSE, nullptr);
	}

}

/// <summary>
/// 描画開始 (画面クリア)
/// </summary>
void Renderer::Begin()
{
	// 画面をクリア (ここでは青でクリア)
	const float clearColor[4] = { 0.0f, 0.4f, 0.75f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}



/// <summary>
/// 描画終了 (画面表示)
/// </summary>
void Renderer::End()
{
	m_swapChain->Present(0, 0);
}


/// <summary>
/// 頂点シェーダーの作成関数
/// </summary>
/// <param name="VertexShader">作成する頂点シェーダーのポインタのポインタ</param>
/// <param name="VertexLayout">作成する頂点レイアウトのポインタのポインタ</param>
/// <param name="FileName">シェーダーファイル名</param>
/// <returns>作成が成功したかの判定</returns>
bool Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{
	// 戻り値用変数
	HRESULT hr = S_OK;

	// ファイル読み込み用変数
	FILE* file;		// ファイルポインタ
	long int fsize;	// ファイルサイズ

	// シェーダーファイルの読み込み
	errno_t err = fopen_s(&file, FileName, "rb");	// ファイルオープン

	// エラーチェック
	if (err != 0 || !file)
	{
		MessageBoxWrapper::errorMessage("シェーダーファイルを開けませんでした");
		return false;
	}

	// ファイルサイズの取得とバッファの確保
	fsize = _filelength(_fileno(file));	// ファイルサイズ取得
	unsigned char* buffer = new unsigned char[fsize];	// ファイルサイズ分のバッファ確保
	fread(buffer, fsize, 1, file);	// ファイル読み込み
	fclose(file);	// ファイルクローズ

	// 頂点シェーダーの作成
	hr = m_device->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	// エラーチェック
	if (!Debug::CheckHR(hr, L"頂点シェーダーの作成に失敗しました")) return false;


	// 頂点レイアウトの作成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// position (float3)  : 0
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,              D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// color (float4)     : 3 * 4 = 12 バイト
		{ "COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 4 * 3,          D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// uv (float2)        : (3+4) * 4 = 28 バイト
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,        0, 4 * 7,          D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// normal (float3)    : (3+4+2) * 4 = 36 バイト
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 4 * 9,          D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// レイアウト要素数の取得
	UINT numElements = ARRAYSIZE(layout);

	// 頂点レイアウトの作成
	m_device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	// バッファ解放
	delete[] buffer;

	return true;	// 作成成功
}


/// <summary>
/// ピクセルシェーダーの作成関数
/// </summary>
/// <param name="PixelShader">作成するピクセルシェーダーのポインタのポインタ</param>
/// <param name="FileName">シェーダーファイル名</param>
/// <returns>作成が成功したかの判定</returns>
bool Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	// 戻り値用変数
	HRESULT hr = S_OK;

	//  ファイル読み込み用変数
	FILE* file = nullptr;		// ファイルポインタ
	long int fsize;				// ファイルサイズ

	// シェーダーファイルの読み込み
	errno_t err = fopen_s(&file, FileName, "rb");	// ファイルオープン
	
	// エラーチェック
	if (err != 0 || !file)
	{
		MessageBoxWrapper::errorMessage("シェーダーファイルを開けませんでした");
		return false;
	}

	// ファイルサイズの取得とバッファの確保
	fsize = _filelength(_fileno(file));	// ファイルサイズ取得
	unsigned char* buffer = new unsigned char[fsize];	// ファイルサイズ分のバッファ確保
	fread(buffer, fsize, 1, file);	// ファイル読み込み
	fclose(file);	// ファイルクローズ

	// ピクセルシェーダーの作成
	hr = m_device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	// エラーチェック
	if (!Debug::CheckHR(hr, L"ピクセルシェーダーの作成に失敗しました")) return false;

	// バッファ解放
	delete[] buffer;

	return true;	// 作成成功
}


//ブレンド変更関数
void Renderer::SetBlendState(BLENDSTATE _blend)
{
	//ブレンドステート設定
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->OMSetBlendState(m_blendState[_blend].Get(), blendFactor, 0xffffffff);
}

//深度ステンシルステート設定関数
void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)	//深度有効
		m_deviceContext->OMSetDepthStencilState(m_depthStateEnable.Get(), NULL);
	else		//深度無効
		m_deviceContext->OMSetDepthStencilState(m_depthStateDisable.Get(), NULL);

}

//2D描画用ワールド・ビュー・プロジェクション行列設定関数
void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

}

//3D描画用ワールド・ビュー・プロジェクション行列設定関数
void Renderer::SetWorldViewProjection3D()
{
	SetWorldMatrix(XMMatrixIdentity());
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
	SetViewMatrix(view);
	XMMATRIX projection;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	SetProjectionMatrix(projection);
}

//3D描画用ワールド・ビュー・プロジェクション行列リセット関数
void Renderer::ResetWorldViewProjection3D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());
	SetProjectionMatrix(XMMatrixIdentity());
}

void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	m_deviceContext->UpdateSubresource(m_worldBuffer.Get(), 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	m_deviceContext->UpdateSubresource(m_viewBuffer.Get(), 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	m_deviceContext->UpdateSubresource(m_projectionBuffer.Get(), 0, NULL, &projectionf, 0, 0);

}
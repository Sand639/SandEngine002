//=======================================================
// ファイル名	: Material.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: マテリアルのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <d3d11.h>          // Direct3D11
#include <wrl/client.h>     // Microsoft::WRL::ComPtr

//=======================================================
// クラス定義
//=======================================================

/// <summary>
/// マテリアルクラス
/// </summary>
/// <remarks>
/// シェーダーやテクスチャなどの描画に関する情報を管理するクラス
/// </remarks>
class Material
{
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:

	// コンストラクタ・デストラクタ
    Material() = default;

    // VS / PS / InputLayout を事前に作って渡す方式
    void SetVertexShader(ID3D11VertexShader* vs) { m_vertexShader = vs; }
    void SetPixelShader(ID3D11PixelShader* ps) { m_pixelShader = ps; }
    void SetInputLayout(ID3D11InputLayout* layout) { m_inputLayout = layout; }

    // GPU にシェーダーをセットする
    void Apply(ID3D11DeviceContext* context);

};
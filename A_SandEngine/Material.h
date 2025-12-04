//=======================================================
// ファイル名	: Material.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 更新日       : 2025/12/04
// 詳細			: マテリアルのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <d3d11.h>          // Direct3D11
#include <wrl/client.h>     // Microsoft::WRL::ComPtr

//=======================================================
// 列挙体定義
//=======================================================

//シェーダーモード
enum SHADERMODE
{
    SHADERMODE_COLOR = 0,    //頂点カラーシェーダー
    SHADERMODE_TEXTURE,      //テクスチャシェーダー
    SHADERMODE_LIGHTING,     //ライティングシェーダー
    SHADERMODE_MAX
};

////ピクセルシェーダー列挙体
//enum PIXELSHADER
//{
//    PIXELSHADER_COLOR = 0,   //頂点カラーシェーダー
//    PIXELSHADER_TEXTURE,     //テクスチャシェーダー
//    PIXELSHADER_LIGHTING,    //ライティングシェーダー
//    PIXELSHADER_MAX
//};
//
////頂点シェーダー列挙体
//enum VERTEXSHADER
//{
//    VERTEXSHADER_COLOR = 0,  //頂点カラーシェーダー
//    VERTEXSHADER_TEXTURE,    //テクスチャシェーダー
//    VERTEXSHADER_LIGHTING,   //ライティングシェーダー
//    VERTEXSHADER_MAX
//};

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

	// 現在のシェーダーモード
    SHADERMODE m_prevShaderMode = SHADERMODE_MAX;

public:

	// コンストラクタ・デストラクタ
    Material() = default;

    // VS / PS / InputLayout を事前に作って渡す方式
    void SetVertexShader(ID3D11VertexShader* vs) { m_vertexShader = vs; }
    void SetPixelShader(ID3D11PixelShader* ps) { m_pixelShader = ps; }
    //void SetInputLayout(ID3D11InputLayout* layout) { m_inputLayout = layout; }

    //シェーダーを変更
    void ChangeShader(SHADERMODE _shaderMode);
	void Apply(ID3D11DeviceContext* context);

};
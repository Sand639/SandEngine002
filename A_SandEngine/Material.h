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
#include <DirectXMath.h>    // DirectXMath
#include <string>           // std::string

//=======================================================
// 前方宣言
//=======================================================
enum BLENDSTATE : int;  //ブレンドステート定義


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
// 構造体定義
//=======================================================
struct CB_Material{
    DirectX::XMFLOAT4 Param0;
    DirectX::XMFLOAT4 Param1;
    DirectX::XMFLOAT4 Param2;
    DirectX::XMFLOAT4 Param3;
};


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

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;  //頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;    //ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;    //入力レイアウト

	// 現在のシェーダーモード
    SHADERMODE m_prevShaderMode;

    //現在のブレンドステート
    BLENDSTATE m_blendState;

	//マテリアル用の定数バッファ
    CB_Material m_cbData{};

public:

    //JSONファイルロード
    bool LoadFromJson(const std::string& path);

	// コンストラクタ・デストラクタ
    Material();
	virtual ~Material() = default;

    //マテリアルの設定を適応する関数
    void Apply();

	//--- セッター・ゲッター ---

    // VS / PS / InputLayout を事前に作って渡す方式
    void SetVertexShader(ID3D11VertexShader* vs) { m_vertexShader = vs; }
    void SetPixelShader(ID3D11PixelShader* ps) { m_pixelShader = ps; }
    void SetInputLayout(ID3D11InputLayout* layout) { m_inputLayout = layout; }

    //シェーダーを変更
    void SetShader(SHADERMODE _shaderMode);

    //ブレンドステートの設定
    void SetBlendState(BLENDSTATE blendState) { m_blendState = blendState; }
    BLENDSTATE GetBlendState() const { return m_blendState; }

	//定数バッファデータの取得
    void SetConstantBufferData(const CB_Material& data) { m_cbData = data; }
    CB_Material& GetConstantBufferData() { return m_cbData; }

	// --- マテリアルパラメータの設定・取得 ---

	//ベースカラーの設定・取得
    void SetBaseColor(const DirectX::XMFLOAT4& color) { m_cbData.Param0 = color; }
    void SetBaseColor(const DirectX::XMFLOAT3& color)
    {
        m_cbData.Param0.x = color.x;
        m_cbData.Param0.y = color.y;
        m_cbData.Param0.z = color.z;
    }
    DirectX::XMFLOAT4 GetBaseColor() const { return m_cbData.Param0; }

	//アルファの設定・取得
    void SetAlpha(float alpha) { m_cbData.Param0.w = alpha; }
    float GetAlpha() const { return m_cbData.Param0.w; }

	//エミッシブカラーの設定・取得
    void SetEmissive(const DirectX::XMFLOAT3& color)
    {
        m_cbData.Param1.x = color.x;
        m_cbData.Param1.y = color.y;
        m_cbData.Param1.z = color.z;
    }
    DirectX::XMFLOAT3 GetEmissive() const
    {
        return DirectX::XMFLOAT3(m_cbData.Param1.x, m_cbData.Param1.y, m_cbData.Param1.z);
	}

	//メタリックの設定・取得
	void SetMetallic(float metallic) { m_cbData.Param1.w = metallic; }
	float GetMetallic() const { return m_cbData.Param1.w; }

	//ラフネスの設定・取得
	void SetRoughness(float roughness) { m_cbData.Param2.x = roughness; }
	float GetRoughness() const { return m_cbData.Param2.x; }




};
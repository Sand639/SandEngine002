//=======================================================
// ファイル名	: common.hlsl
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/04
// 更新日		: 2025/12/04
// 概要			: シェーダー共通ヘッダー
//=======================================================

//=======================================================
// 構造体
//=======================================================

// マテリアル構造体
struct MATERIAL
{
    float4 Param0;
    float4 Param1;
    float4 Param2;
    float4 Param3;
};

//=======================================
// 定数バッファ
//=======================================

// ワールド行列
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

// ビュー行列
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

// プロジェクション行列
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// マテリアル定数バッファ
cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
};


//=======================================================
// シェーダー入出力構造体
//=======================================================

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 position : POSITION0;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 position : SV_Position;
    float4 worldPosition : POSITION0;
    float4 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texCoord : TexCOORD0;
    float depth : DEPTH0; //深度値
};


//=======================================================
// ゲッター
//=======================================================

// ベースカラー取得
float4 GetBaseColor()
{
    return Material.Param0;
}

// アルファ取得
float GetAlpha()
{
    return Material.Param0.a;
}

// エミッシブカラー取得
float3 GetEmissive()
{
    return Material.Param1.rgb;
}

// メタリック取得
float GetMetallic()
{
    return Material.Param1.a;
}

// ラフネス取得
float GetRoughness()
{
    return Material.Param2.x;
}

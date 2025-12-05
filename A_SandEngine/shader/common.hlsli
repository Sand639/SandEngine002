//=======================================================
// ファイル名	: common.hlsl
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/04
// 更新日		: 2025/12/04
// 概要			: シェーダー共通ヘッダー
//=======================================================

//=======================================================
// 定数バッファ構造体
//=======================================================

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


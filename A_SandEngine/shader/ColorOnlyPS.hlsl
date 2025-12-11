//=======================================================
// ファイル名	: ColorOnlyPS.hlsl
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/04
// 更新日		: 2025/12/04
// 概要			: 色のみピクセルシェーダー
//=======================================================
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "common.hlsli" 

// ピクセルシェーダーメイン関数
void PSMain(in VS_OUT In, out float4 outDiffuse : SV_Target)
{
    // ベースカラーとパラメータをマテリアルCBから取得
    float4 baseColor = GetBaseColor();
    float metallic = GetMetallic(); // 0 = 非金属, 1 = 完全な金属
    float roughness = saturate(GetRoughness()); // 念のため 0～1 にクランプ
    
    // 簡易的な拡散反射の調整（粗さに応じて暗くする）
    float diffuseFactor = lerp(1.0f, 0.4f, roughness);
    
      // メタリックが高いほど「色を強める」＋少し白寄りのハイライトを足す
    float3 metalHighlight = lerp(0.0.xxx, 0.3.xxx, metallic);

    float3 color = baseColor.xyz * diffuseFactor + metalHighlight;

    // エミッシブも足したければここで加算
    color += GetEmissive();

    outDiffuse = float4(color, GetAlpha());
     
}
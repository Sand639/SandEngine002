//=======================================================
// ファイル名	: ColorOnlyVS.hlsl
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/04
// 更新日		: 2025/12/04
// 概要			: 色のみ頂点シェーダー
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "common.hlsli"

// 頂点シェーダーメイン関数
void VSMain(in VS_IN In, out VS_OUT Out)
{

    // 座標変換なしでそのまま出力
    Out.position = float4(In.position.xy, 0.5f, 1.0f);
    
    //
    Out.color = GetBaseColor();
    
    //matrix wvp;
    //wvp = mul(World, View);
    //wvp = mul(wvp, Projection);

    //Out.position = mul(In.position, wvp);
    //Out.texCoord = In.texCoord;

    ////Out.Depth = Out.Position.z; //深度値を計算して出力
}

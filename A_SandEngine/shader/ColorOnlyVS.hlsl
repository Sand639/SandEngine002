//=======================================================
// ファイル名	: common.hls
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/12/04
// 更新日		: 2025/12/04
// 概要			: シェーダー共通ヘッダー
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "common.hlsli"

// 頂点シェーダーメイン関数
void VSMain(in VS_IN In, out VS_OUT Out)
{

    Out.position = float4(In.position.xyz, 1.0f); // そのままクリップ空間へ
    Out.color = In.color;
    
    //matrix wvp;
    //wvp = mul(World, View);
    //wvp = mul(wvp, Projection);

    //Out.position = mul(In.position, wvp);
    //Out.texCoord = In.texCoord;
    //Out.color = In.color * Material.Diffuse;
    ////Out.Depth = Out.Position.z; //深度値を計算して出力
}


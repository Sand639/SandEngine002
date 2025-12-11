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

    // 位置を float4 に（w = 1）
    float4 localPos = float4(In.position, 1.0f);

    // WVP 行列を作成
    matrix wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    // クリップ空間へ変換
    Out.position = mul(localPos, wvp);

    // ワールド座標も一応出しておく（あとでライトで使える）
    Out.worldPosition = mul(localPos, World);

    // 法線もワールド空間に
    Out.normal = normalize(mul(float4(In.normal, 0.0f), World));

    // カラーは今まで通りマテリアルから
    Out.color = In.color;

    // UV もそのまま通しておく（将来テクスチャ用）
    Out.texCoord = In.texCoord;

    // 深度値
    Out.depth = Out.position.z;
}

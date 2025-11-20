//=======================================================
// ファイル名	: Mesh.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: メッシュのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <DirectXMath.h>	// DirectXMath (数学ライブラリ)
#include <wrl/client.h>     // Microsoft::WRL::ComPtr
#include <d3d11.h>          // Direct3D11
#include <vector>           // std::vector

//頂点構造体
struct Vertex
{
	DirectX::XMFLOAT3 position; //位置
	DirectX::XMFLOAT3 normal;   //法線
	DirectX::XMFLOAT2 uv;       //UV座標
	DirectX::XMFLOAT4 color;    //頂点カラー
};


/// <summary>
/// メッシュクラス
/// </summary>
/// <remarks>
/// 頂点バッファ、インデックスバッファを管理し、描画を行うクラス
/// </remarks>
class Mesh
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;		//インデックスバッファ
	UINT m_vertexCount = 0;	//頂点数
	UINT m_indexCount = 0;	//インデックス数
	UINT m_stride = 0;		//頂点のストライド

public:

	//コンストラクタ・デストラクタ
	Mesh() = default;
	~Mesh() = default;

	// 初期化関数
    bool Init(
        ID3D11Device* device,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices);

	// 描画関数
    void Draw(ID3D11DeviceContext* context);



};
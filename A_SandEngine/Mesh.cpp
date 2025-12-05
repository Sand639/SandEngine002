//=======================================================
// ファイル名	: Mesh.cpp
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: メッシュの実装ファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Mesh.h"
#include "Renderer.h"
#include "Debug.h"


/// <summary>
/// 初期化関数
/// </summary>
/// <param name="device">D3D11デバイス</param>
/// <param name="vertices">頂点データ配列</param>
/// <param name="indices">インデックスデータ配列</param>
/// <returns>初期化が成功したかの判定</returns>
bool Mesh::Init(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_vertexCount = static_cast<UINT>(vertices.size()); // 頂点数を保存
	m_indexCount = static_cast<UINT>(indices.size());   // インデックス数を保存
	m_stride = sizeof(Vertex);          	            // 頂点のストライドを保存  

    // 1. 頂点バッファ作成
	D3D11_BUFFER_DESC vbd = {};                         // 頂点バッファの設定構造体
	vbd.Usage = D3D11_USAGE_DEFAULT;                    // 使用方法の設定
	vbd.ByteWidth = sizeof(Vertex) * m_vertexCount;     // バッファのサイズ
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;           // バインドフラグを頂点バッファに設定

	D3D11_SUBRESOURCE_DATA vInit = {};                  // 頂点バッファの初期化データ構造体
	vInit.pSysMem = vertices.data();                    // 頂点データのポインタを設定

	// 頂点バッファの作成
    HRESULT hr = device->CreateBuffer(&vbd, &vInit, m_vertexBuffer.ReleaseAndGetAddressOf());

    // エラーチェック
    if (!Debug::CheckHR(hr, L"頂点バッファの作成に失敗しました")) return false;


    // 2. インデックスバッファ作成
    D3D11_BUFFER_DESC ibd = {};                         // インデックスバッファの設定構造体
    ibd.Usage = D3D11_USAGE_DEFAULT;                    // 使用方法の設定
    ibd.ByteWidth = sizeof(uint32_t) * m_indexCount;    // バッファのサイズ
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;            // バインドフラグをインデックスバッファに設定

	D3D11_SUBRESOURCE_DATA iInit = {};                  // インデックスバッファの初期化データ構造体
	iInit.pSysMem = indices.data();                     // インデックスデータのポインタを設定

	// インデックスバッファの作成
    hr= device->CreateBuffer(&ibd, &iInit, m_indexBuffer.ReleaseAndGetAddressOf());

    // エラーチェック
    if (!Debug::CheckHR(hr, L"インデックスバッファの作成に失敗しました")) return false;

    return true;
}


/// <summary>
/// 描画関数
/// </summary>
/// <param name="context">D3D11デバイスコンテキスト</param>
/// returns>なし</returns>
void Mesh::Draw(ID3D11DeviceContext* context)
{
    //Debug::Log("Mesh Draw");

    UINT offset = 0;

	// パイプラインに頂点バッファ、インデックスバッファを設定
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	// プリミティブトポロジーの設定（三角形リスト）
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// インデックス描画の実行
    context->DrawIndexed(m_indexCount, 0, 0);
}



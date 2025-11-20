//=======================================================
// ファイル名	: Material.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 詳細			: マテリアルのヘッダファイル
//=======================================================
#include "Material.h"

void Material::Apply(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    context->IASetInputLayout(m_inputLayout.Get());
}

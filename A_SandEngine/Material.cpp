//=======================================================
// ファイル名	: Material.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 更新日       : 2025/12/04
// 詳細			: マテリアルのヘッダファイル
//=======================================================

//=======================================================
// インクルード
//=======================================================
#include "Material.h"
#include "Renderer.h"
#include "GameEngine.h"
#include "Debug.h"

void Material::ChangeShader(SHADERMODE _shaderMode)
{
	//前回のモードと今回のモードならそのまま使用
	if (m_prevShaderMode == _shaderMode)return;

	//シェーダーモードによってシェーダーを変更
	switch (_shaderMode)
	{
		case SHADERMODE_COLOR:
		//頂点カラーシェーダーに変更
		GameEngine::GetInstance().GetRenderer()->CreateVertexShader(m_vertexShader.GetAddressOf(), m_inputLayout.GetAddressOf(),  "shader\\.cso\\ColorOnlyVS.cso");
		GameEngine::GetInstance().GetRenderer()->CreatePixelShader(m_pixelShader.GetAddressOf(), "shader\\.cso\\ColorOnlyPS.cso");
			break;
	}

	//シェーダーモードを保存
	m_prevShaderMode = _shaderMode;
}

void Material::Apply(ID3D11DeviceContext* context)
{

	if (!m_vertexShader)
	{
		Debug::LogWarning("Material::Apply - 頂点シェーダーが設定されていません");
		return;
	}

	if (!m_pixelShader)
	{
		Debug::LogWarning("Material::Apply - ピクセルシェーダーが設定されていません");
		return;
	}

	if (!m_inputLayout)
	{
		Debug::LogWarning("Material::Apply - 入力レイアウトが設定されていません");
		return;
	}

	//シェーダーをセット
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	//入力レイアウトをセット
	context->IASetInputLayout(m_inputLayout.Get());

	//Debug::Log("Material Apply");
}

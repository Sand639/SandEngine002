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

//コンストラクタ
Material::Material() :
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_inputLayout(nullptr),
	m_prevShaderMode(SHADERMODE_MAX),
	m_blendState(BLENDSTATE_ALFA)
{
	//デフォルトのマテリアルパラメータを設定
	m_cbData.Param0 = { 1.0f, 1.0f, 1.0f, 1.0f }; // BaseColor + Alpha
	m_cbData.Param1 = { 0.0f, 0.0f, 0.0f, 0.0f }; // Emissive + Metallic
	m_cbData.Param2 = { 1.0f, 0.0f, 0.0f, 0.0f }; // Roughness=1 とか
}

void Material::SetShader(SHADERMODE _shaderMode)
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



/// <summary>
/// マテリアルの設定を適応する関数
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
void Material::Apply()
{
	//シェーダーが設定されていない場合、警告を出してデフォルトのシェーダーを設定
	if (!m_vertexShader || !m_pixelShader || !m_inputLayout)
	{
		Debug::LogWarning("Material::Apply - シェーダーが設定されていません");
		
		//デフォルトのシェーダーを設定
		SetShader(SHADERMODE_COLOR);
	}

	auto renderer = GameEngine::GetInstance().GetRenderer();
	auto context = renderer->GetDeviceContext();

	//シェーダーをセット
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	//入力レイアウトをセット
	context->IASetInputLayout(m_inputLayout.Get());

	//ブレンドステートをセット
	renderer->SetBlendState(m_blendState);


	//マテリアル用定数バッファをセット
	renderer->SetMaterial(m_cbData);
}

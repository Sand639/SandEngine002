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
#include "Material.h"			// マテリアルクラス
#include "Renderer.h"			// レンダラークラス
#include "GameEngine.h"			// ゲームエンジンクラス
#include "Debug.h"				// デバッグクラス
#include <fstream>				// ifstream 用
#include "External/json.hpp"	// nlohmann::json 用

//=======================================================
// 型エイリアス
//=======================================================
using json = nlohmann::json;

//=======================================================
// 名前空間
//=======================================================
namespace {

	/// <summary>
	// 文字列からシェーダーモードを取得する関数
	/// </summary>
	/// <param name="s">文字列</param>
	/// <returns>シェーダーモード</returns>
	SHADERMODE ShaderModeFromString(const std::string& s)
	{
		// シェーダーモードの判定
		if (s == "SHADERMODE_COLOR")   return SHADERMODE_COLOR;
		if (s == "SHADERMODE_TEXTURE") return SHADERMODE_TEXTURE;
		if (s == "SHADERMODE_LIGHTING")return SHADERMODE_LIGHTING;

		// 想定外の文字列が来たとき
		Debug::LogWarning("存在しないshaderModeがJSONファイルに設定されています : " + s);
		return SHADERMODE_COLOR; // デフォルト
	}

	/// <summary>
	// 文字列からブレンドステートを取得する関数
	/// </summary>
	/// <param name="s">文字列</param>
	/// <returns>ブレンドステート</returns>
	BLENDSTATE BlendStateFromString(const std::string& s)
	{
		// ブレンドステートの判定
		if (s == "BLENDSTATE_NONE") return BLENDSTATE_NONE;
		if (s == "BLENDSTATE_ALFA") return BLENDSTATE_ALFA;
		if (s == "BLENDSTATE_ADD")  return BLENDSTATE_ADD;
		if (s == "BLENDSTATE_SUB")  return BLENDSTATE_SUB;
		if (s == "BLENDSTATE_MASK") return BLENDSTATE_MASK;

		// 想定外の文字列が来たとき
		Debug::LogWarning("存在しないblendStateがJSONファイルに設定されています : " + s);
		return BLENDSTATE_ALFA; // デフォルト
	}

}

/// <summary>
//コンストラクタ
/// </summary>
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

/// <summary>
/// シェーダーを変更する関数
/// </summary>
/// <param name="_shaderMode">シェーダーモード</param>
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

/// <summary>
/// JSONファイルからマテリアルをロードする関数
/// </summary>
/// <param name="path">JSONファイルのパス</param>
/// <returns>ロード成功ならtrue、失敗ならfalse</returns>
bool Material::LoadFromJson(const std::string& path)
{
	// 1. ファイルを開く
	std::ifstream f(path);
	if (!f)
	{
		Debug::LogError("MaterialのJSONファイルが開けません : " + path);
		return false;
	}

	try
	{
		// 2. JSON としてパース
		json data = json::parse(f);

		// 3. JSON から Materialクラス に値を詰める

		//シェーダーモードの取得
		auto shaderModeStr = data.value("shaderMode", "SHADERMODE_COLOR");
		SetShader(ShaderModeFromString(shaderModeStr));

		// ブレンドステートの取得
		auto blendStateStr = data.value("blendState", "BLENDSTATE_ALFA");
		SetBlendState(BlendStateFromString(blendStateStr));

		// params オブジェクトを取得
		json params = data.value("params", json::object());

		// ベースカラーの取得
		auto color = params.value("BaseColor", std::vector<float>{1, 1, 1, 1});
		SetBaseColor({ color[0], color[1], color[2], color[3] });

		// エミッシブカラーの取得
		auto emissive = params.value("Emissive", std::vector<float>{0, 0, 0});
		SetEmissive({ emissive[0], emissive[1], emissive[2] });

		//メタリックの取得
		auto metallic = params.value("Metallic", 0.0f);
		SetMetallic(metallic);

		//ラフネスの取得
		auto Roughness = params.value("Roughness", 1.0f);
		SetRoughness(Roughness);

		return true;
	}
	catch (const std::exception& e)
	{
		Debug::LogError(std::string("Material JSON parse error: " + std::string(e.what())));
		return false;
	}
}
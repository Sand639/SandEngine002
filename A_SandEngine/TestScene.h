//=======================================================
// ファイル名	: TestScene.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/27
// 更新日		: 2025/11/27
// 詳細			: テストシーンクラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include "Scene.h"  // シーンクラス

class Entity;
class Mesh;
class MeshRenderer;

/// <summary>
/// テストシーンクラス
/// </summary>
/// <remarks>
/// 複数のEntityを管理し、それらのライフサイクルを実行します。
/// </remarks>
class TestScene : public Scene
{
private:
    //テスト用のエンティティ
    std::shared_ptr<Entity> m_testEntity;

     //   std::shared_ptr<Mesh> m_mesh;

	//std::shared_ptr<MeshRenderer> m_meshRenderer;

public:

    //コンストラクタ
    TestScene(const std::string& name = "New Scene") : Scene(name) {}
    //デストラクタ
    virtual ~TestScene() = default;

    //ライフサイクル関数

    void Init() override;

    void Awake() override;
    void Start() override;
    void Uninit() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void Draw() override;
    void EndOfFrame() override;

};

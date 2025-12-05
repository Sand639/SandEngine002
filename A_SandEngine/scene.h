//=======================================================
// ファイル名	: Scene.h
// 制作者		: 大槻 海斗(Sand)
// 制作日		: 2025/11/20
// 更新日		: 2025/11/20
// 詳細			: シーンクラスのヘッダファイル
//=======================================================
#pragma once
//=======================================================
// インクルード
//=======================================================
#include <vector>       // std::vector
#include <memory>       // std::shared_ptr
#include <string>       // std::string
#include "Entity.h" // ゲームオブジェクトクラス

/// <summary>
/// シーンクラス
/// </summary>
/// <remarks>
/// 複数のEntityを管理し、それらのライフサイクルを実行します。
/// </remarks>
class Scene
{
protected:
    //シーンの名前
    std::string m_name;
    // このシーンに存在する全てのEntityを所有・管理
    std::vector<std::shared_ptr<Entity>> m_entities;

public:

	//コンストラクタ
    Scene(const std::string& name = "New Scene");
	//デストラクタ
    virtual ~Scene() = default;

    //ライフサイクル関数

    virtual void Init();

    virtual void Awake();
    virtual void Start();
    virtual void Uninit();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdate();
    virtual void Draw();
    virtual void EndOfFrame();


    // セッター

    // シーンの名前を設定
    void SetName(const std::string& name) { m_name = name; }


    // ゲッター

    // シーンの名前を取得
    const std::string& GetName() const { return m_name; }

    // --- Entity管理 ---

    // Entity追加関数
    template <typename T, typename... Args>
    std::shared_ptr<T> AddEntity(Args&&... args);

    //特定の型のEntityを取得する関数
    template <typename T>
	std::vector<std::shared_ptr<T>> GetEntity();

	// シーン内の全エンティティを取得
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const {
        return m_entities;
    }

};


/// <summary>
/// シーンにエンティティを追加する関数
/// </summary>
template<typename T, typename... Args>
inline std::shared_ptr<T> Scene::AddEntity(Args&&... args)
{
	std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(args)...);

    m_entities.push_back(entity);
    return entity;
}

/// <summary>
/// シーン内の特定の型の全エンティティを取得する関数
/// </summary>
template<typename T>
inline std::vector<std::shared_ptr<T>> Scene::GetEntity()
{
    // T は Entity の派生型であることをコンパイル時チェック
    static_assert(std::is_base_of<Entity, T>::value, "T must inherit from Entity");

	// 結果を格納するベクター
    std::vector<std::shared_ptr<T>> finds;
	finds.reserve(m_entities.size());   // 予め容量を確保しておくと効率的

	// 全エンティティを走査
    for (auto& entity : m_entities) {
		// 動的キャストを試みる
        if (auto casted = std::dynamic_pointer_cast<T>(entity))
        {
			// キャスト成功、リストに追加
            finds.push_back(casted);
        }
    }

    return finds;
}

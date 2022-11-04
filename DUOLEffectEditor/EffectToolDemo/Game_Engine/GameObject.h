#pragma once

/// <summary>
/// ������ �ܼ��ϰ� ������
/// ������ ������!
/// 2022-03-29 �ż���
/// </summary>
namespace Muscle
{
	class IComponents;
	class Transform;
	class Collider;

	class GameObject : public std::enable_shared_from_this<GameObject>, public MonoBehavior
	{

	public:
		GameObject();
		virtual ~GameObject();

	public:
		//������Ʈ
		virtual void Start() override; // ������Ʈ �Ŵ������� ���� ��.

		virtual void Update() override; // ������Ʈ �Ŵ������� ���� ��.

		virtual void LateUpdate() override; // ������Ʈ �Ŵ������� ���� ��.

		virtual void Render() override; // ������Ʈ �Ŵ������� ���� ��.

		inline std::shared_ptr<Transform> GetTransform() { return m_Transform; }

		void SetTransform(std::shared_ptr<Transform> _transform);

		void SetParent(std::shared_ptr<GameObject> _GameObject);

		void SetChild(std::shared_ptr<GameObject> _GameObject);

		std::shared_ptr<GameObject> GetParent() { return m_Parent.lock(); }

		const std::vector<std::shared_ptr<GameObject>>& GetChildrens() { return m_Childrens; }

		void GetAllChildrens(std::vector<std::shared_ptr<GameObject>>& out);

		void SetName(std::string _Name) { m_Name = _Name; };

		void SetTag(std::string _Tag);

		void SetName(std::wstring _Name);

		const std::string& GetTag() { return m_Tag; }

		const std::string& GetName() { return m_Name; }

		bool GetIsEnable();

		void SetIsEnable(bool value);

		std::shared_ptr<GameObject> GetTopParent();

		std::shared_ptr<GameObject> FindChildren(std::string _Name);

		std::shared_ptr<GameObject> FindChildren(std::shared_ptr<GameObject> child);

		void DeleteChildren(std::shared_ptr<GameObject> child);

		void Finalize();

	private:

		std::string m_Name;

		std::string m_Tag; // �±װ� ������ �浹 ���Ѵ�..! // �±װ� ��� ������ ��� �±׿� �浹.

		std::shared_ptr<Transform> m_Transform;

		std::weak_ptr<GameObject> m_Parent;

		std::vector<std::shared_ptr<GameObject>> m_Childrens;

		bool m_isRender;

		bool m_isEnable;

		UINT m_ObjectID;

	public:
		inline bool GetIsRender() { return m_isRender; }

		void SetIsRender(bool _bool);

		const UINT GetObjectID() { return m_ObjectID; }

	public:
		virtual void OnEnterCollision(std::shared_ptr<Collider> _Other) override;

		virtual void OnStayCollision(std::shared_ptr<Collider> _Other) override;

		virtual void OnExitCollision(std::shared_ptr<Collider> _Other) override;

	public:
		//������Ʈ ��� ���� �� �Լ���
		std::vector<std::shared_ptr<IComponents>> m_Components;

		template  <class  TComponent>
		std::shared_ptr<TComponent> AddComponent();

		template  <class  TComponent>
		std::shared_ptr<TComponent> GetComponent();

		std::vector<std::shared_ptr<IComponents>>& GetComponents() { return m_Components; }

		friend class ObjectManager;
	};

	template<class TComponent>
	std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		std::shared_ptr<TComponent> tempComponent = std::make_shared<TComponent>(this->shared_from_this());

		m_Components.emplace_back(tempComponent);

		return tempComponent;
	}

	template<class TComponent>
	std::shared_ptr<TComponent> GameObject::GetComponent()
	{
		for (auto iter : m_Components)
		{
			std::shared_ptr<TComponent> tempComponent = std::dynamic_pointer_cast<TComponent>(iter);

			if (tempComponent != nullptr)
			{
				return tempComponent;
			}
		}

		return nullptr;
	}
	//#endif

	extern std::shared_ptr<GameObject> CreateGameObject();

	extern std::shared_ptr<GameObject> CreateFromPrefab(const tstring& prefabName);

	extern void DeleteGameObject(std::shared_ptr<GameObject> gameObject);
}
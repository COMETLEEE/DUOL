#pragma once
#include <algorithm>
#include <vector>

#include "DUOLGameEngine/ECS/Component/ObjectBase.h"

// For define template 'Add / Get' Component functions.
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class ComponentBase;

	class BehaviourBase;

	class MonoBehaviourBase;

	class Transform;

	class Scene;

	/**
	 * \brief 게임 내의 엔티티를 나타내는 클래스입니다.
	 */
	class GameObject : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * \brief 게임 오브젝트 객체를 만듭니다. 다만, 만약 해당 원시 포인터에 대해서 shared_ptr 객체를
		 * 만들지 않고 shared_from_this를 호출하면 Exception이 발생합니다.
		 * 따라서, 정석은 생성자를 private로 막고, shared_ptr을 반환하는 CreateEmpty() 등의 함수를 열어두어야 합니다.
		 * \param name 해당 게임 오브젝트의 이름입니다.
		 */
		GameObject(const DUOLCommon::tstring& name);

		virtual ~GameObject() override;

		DEFINE_DEFAULT_COPY_MOVE(GameObject)

	private:
		std::shared_ptr<DUOLGameEngine::Transform> _transform;

		std::shared_ptr<DUOLGameEngine::MonoBehaviourBase> _monoBehaviour;

		/**
		 * \brief 활성화 되어 있는 모든 컴포넌트들의 리스트
		 */
		std::vector<std::shared_ptr<DUOLGameEngine::ComponentBase>> _components;

		/**
		 * \brief 비활성화 되어 있는 모든 Behaviour들의 리스트
		 */
		std::vector<std::shared_ptr<DUOLGameEngine::BehaviourBase>> _disabledBehaviours;

	public:
		inline const std::shared_ptr<DUOLGameEngine::Transform>& GetTransform() const { return _transform; }

		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent() const;

		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

	private:
		/**
		 * \brief 게임 오브젝트를 해제합니다.
		 */
		void UnInitialize();

	private:
		/**
		 * \brief Behaviour를 활성화될 때 호출됩니다. Loop를 시작합니다.
		 * \param target 활성화되는 Behaviour
		 */
		void SetBehaviourEnabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);

		/**
		 * \brief Behaviour가 비활성화될 때 호출됩니다. Loop를 정지합니다.
		 * \param target 비활성화되는 Behaviour
		 */
		void SetBehaviourDisabled(const std::shared_ptr<DUOLGameEngine::BehaviourBase>& target);

	public:
		/**
		* \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다. 비활성화 게임 오브젝트도 실행합니다.
		* 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		*/
		virtual void OnAwake();

		/**
		 * \brief 씬이 시작할 때 OnAwake 이후에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		virtual void OnStart();

		/**
		 * \brief 게임 오브젝트가 활성화될 때 호출됩니다.
		 */
		virtual void OnActive();

		/**
		 * \brief 게임 오브젝트가 비활성화될 때 호출됩니다.
		 */
		virtual void OnInActive();

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy();

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * \brief 만약, 해당 게임 오브젝트의 MonoBehaviourBase를 상속한 스크립트에서 이전에 yield 했지만 이제 Resume 할
		 * Coroutine이 있다면 해당 함수에서 실시합니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnCoroutineUpdate(float deltaTime);

		/**
		 * \brief 매 물리 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnFixedUpdate(float deltaTime);

		/**
		 * \brief OnUpdate 이후, 매 프레임 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnLateUpdate(float deltaTime);

	private:
		/**
		 * \brief 해당 게임 오브젝트가 속해있는 씬입니다.
		 */
		std::weak_ptr<DUOLGameEngine::Scene> _scene;

		/**
		 * \brief 게임 오브젝트의 태그입니다. Tag와 Layer Manager에서 셋팅 후 사용합니다.
		 */
		DUOLCommon::tstring _tag;

		/**
		 * \brief 게임 오브젝트의 레이어입니다. 레이어는 선택적인 렌더링 및 레이 캐스트에 사용할 수 있습니다.
		 */
		int _layer;

		/**
		 * \brief 게임 오브젝트가 활성화 상태인지 나타냅니다.
		 */
		bool _isActive;

	public:
		inline std::shared_ptr<DUOLGameEngine::Scene> GetScene() const { return _scene.lock(); }

		inline const DUOLCommon::tstring& GetTag() const { return _tag; }

		inline void SetTag(const DUOLCommon::tstring& tag) { _tag = tag; }

		inline int GetLayer() const { return _layer; }

		inline void SetLayer(int layer) { _layer = layer; }

		inline bool GetIsActive() const { return _isActive; }

		void SetIsActive(bool value);
#pragma region FRIEND_CLASS
		friend class ComponentBase;

		friend class BehaviourBase;

		friend class Scene;
#pragma endregion
	};

#pragma region GET_ADD_COMPONENT
	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value,
			"TComponent must inherit from ComponentBase");

		// 트랜스폼 캐싱 (이것 사용은 막았으면 좋겠다 .. GetTransform 있으니까 ..
		if constexpr (std::is_same_v<TComponent, Transform>)
			return _transform;

		// 모노 비해이비어 캐싱
		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
			return _monoBehaviour;

		// TODO
		// 아무래도 자동화와 효율성을 동시에 추구하기 위해서는 .. 리플렉션 기능이 필요할 것 같습니다.
#pragma region USING_DYNAMIC_CAST
		for (int i = 0 ; i < _components.size() ; i++)
		{
			std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(_components[i]);

			if (dcComponent != nullptr)
				return dcComponent;
		}

		for (int i = 0; i < _disabledBehaviours.size(); i++)
		{
			std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(_disabledBehaviours[i]);

			if (dcComponent != nullptr)
				return dcComponent;
		}

		// 없음. nullptr 반환.
		return nullptr;
#pragma endregion
	}

	// 컴포넌트는 해당 함수를 통해서만 객체화된다.
	template <typename TComponent>
	std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<ComponentBase, TComponent>,
			"TComponent must inherit from ComponentBase");

		TComponent* primitiveCom = new TComponent(this->weak_from_this());

		// TODO
		// Resource (Memory) 관리를 위한 Deleter를 매개변수로 넣어줄 수 있다.
		std::shared_ptr<TComponent> component(primitiveCom);

		if constexpr (std::is_base_of_v<MonoBehaviourBase, TComponent>)
		{
			_monoBehaviour = component;
		}

		if constexpr (std::is_same_v<TComponent, Transform>)
		{
			// 트랜스폼은 따로 캐싱한다.
			_transform = component;
		}

		_components.push_back(component);

		return component;
	}
#pragma endregion
}
#pragma once
#include <cassert>

#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief 게임 오브젝트에 붙을 수 있는 모든 컴포넌트들의 부모 클래스
	 */
	class ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief 상속받은 클래스에서 반드시 호출되어야 합니다.
		 * \param owner 해당 컴포넌트를 소유한 게임 오브젝트
		 * \param name 해당 컴포넌트의 이름
		 */
		ComponentBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Component"));

		/**
		 * \brief 다형성을 유지한 소멸자
		 */
		virtual ~ComponentBase() override;

		DEFINE_DEFAULT_COPY_MOVE(ComponentBase)

		/**
		 * \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		virtual void OnAwake() {}

		/**
		 * \brief 씬이 시작할 때 OnAwake 이후에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		virtual void OnStart() {}

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy() {}

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnUpdate(float deltaTime) {}

		/**
		 * \brief 매 물리 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnFixedUpdate(float deltaTime) {}

		/**
		 * \brief OnUpdate 이후, 매 프레임 호출됩니다. 
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnLateUpdate(float deltaTime) {}

	private:
		/**
		 * \brief 해당 컴포넌트를 소유하고 있는 게임 오브젝트입니다.
		 * 순환 참조를 막기 위해 std::weak_ptr로 선언합니다.
		 */
		std::weak_ptr<GameObject> _owner;

		/**
		 * \brief 해당 오브젝트의 Transform Component입니다.
		 */
		std::shared_ptr<Transform> _transform;
		
	public:
		inline std::shared_ptr<GameObject> GetGameObject() const
		{
			std::shared_ptr<GameObject> owner = _owner.lock();

			assert(owner != nullptr);

			return owner;
		}

		inline const std::shared_ptr<Transform>& GetTransform() const
		{
			return _transform;
		}

		const DUOLCommon::tstring& GetTag() const;

		bool CompareTag(const DUOLCommon::tstring& tag) const;
 	};
}
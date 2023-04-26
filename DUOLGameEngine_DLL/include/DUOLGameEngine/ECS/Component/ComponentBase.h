#pragma once
#include <cassert>

#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLCommon/Event/Event.h"

#include "rttr/registration_friend.h"
#include "rttr/rttr_enable.h"

namespace DUOLEditor
{
	class Inspector;
}

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief 게임 오브젝트에 붙을 수 있는 모든 컴포넌트들의 부모 클래스
	 */
	class DUOL_GAMEENGINE_API ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief 기본 생성자
		 */	
		ComponentBase();

		/**
		 * \brief 상속받은 클래스에서 반드시 호출되어야 합니다.
		 * \param owner 해당 컴포넌트를 소유한 게임 오브젝트
		 * \param name 해당 컴포넌트의 이름
		 */
		ComponentBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Component"));

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
		 * \param fixedTimeStep 프레임 간 시간 간격입니다. 고정된
		 */
		virtual void OnFixedUpdate(float fixedTimeStep) {}

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
		DUOLGameEngine::GameObject* _owner;

	public:
		inline GameObject* GetGameObject() const
		{
			assert(_owner != nullptr);

			return _owner;
		}

		Transform* GetTransform() const;

		const DUOLCommon::tstring& GetTag() const;

		bool CompareTag(const DUOLCommon::tstring& tag) const;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class DUOLGameEngine::GameObject;
#pragma endregion
 	};
}
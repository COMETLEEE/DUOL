#pragma once
#include "ObjectBase.h"
#include "DUOLGameEngine/Util/Constants.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief 액터에 붙을 수 있는 모든 컴포넌트들의 부모 클래스
	 */
	class ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief 상속받은 클래스에서 반드시 호출되어야 합니다.
		 * \param actor 
		 */
		ComponentBase(std::shared_ptr<GameObject> owner);

		/**
		 * \brief 다형성을 유지한 소멸자
		 */
		virtual ~ComponentBase();

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
		 * \brief 컴포넌트가 사용 가능해졌을 때 호출됩니다.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief 컴포넌트가 사용 불가능해졌을 때 호출됩니다.
		 */
		virtual void OnDisable() {}

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy() {}

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 
		 */
		virtual void OnUpdate(float deltaTime) {}

		/**
		 * \brief 매 물리 프레임에 호출됩니다.
		 * \param deltaTime 
		 */
		virtual void OnFixedUpdate(float deltaTime) {}

		/**
		 * \brief OnUpdate 이후, 매 프레임 호출됩니다. 
		 * \param deltaTime 
		 */
		virtual void OnLateUpdate(float deltaTime) {}

	private:
		std::shared_ptr<GameObject> _owner;

		std::shared_ptr<Transform> _transform;

		/**
		 * \brief 게임 오브젝트의 태그입니다. Tag와 Layer Manager에서 셋팅 후 사용합니다.
		 */
		tstring _tag;

	public:

 	};
}
/**

    @file      PhysicsManager.h
    @brief     물리와 관련된 흐름을 담당하는 매니저. Physics Engine과 연동한다.
    @details   ~
    @author    COMETLEE
    @date      4.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

#include "DUOLCommon/Event/Event.h"

// Physics engine headers.
#include "DUOLPhysics/System/PhysicsSystem.h"

namespace DUOLGameEngine
{
	class Transform;

	class GameObject;

	class ColliderBase;

	class Rigidbody;
}

namespace DUOLGameEngine
{
	// TODO : 씬에서 새로운 게임 오브젝트가 생성되는데 만약 .. 물리 관련된 오브젝트라면
	// TODO : PhysicsManager에게 콜백 요청할 수 있도록 해야합니다.

	/**
	 * \brief 물리 시뮬레이션 담당하는 매니저입니다.
	 */
	class PhysicsManager final : public SingletonBase<PhysicsManager>
	{
		DECLARE_SINGLETON(PhysicsManager)

		DELETE_COPY_MOVE(PhysicsManager)

	private:
        virtual ~PhysicsManager() override;

        float _fixedTimeStep;

#pragma region PHYSICS_SYSTEM
		/**
		 * \brief PhysX Engine Wrapper.
		 */
		std::shared_ptr<DUOLPhysics::PhysicsSystem> _physicsSystem;

		/**
		 * \brief DUOLGameEngine의 물리 시뮬레이션이 진행되는 씬 => 지금 생각해보니
		 * 다중 씬을 위해서라도 씬마다 물리 씬이 있어야할 것 같다 ..
		 */
		std::weak_ptr<DUOLPhysics::PhysicsScene> _physicsScene;

		/**
		 * \brief 현재 씬 상의 Static Actors
		 */
		std::unordered_map<DUOLCommon::tstring,
			std::pair<std::weak_ptr<DUOLGameEngine::Transform>, std::weak_ptr<DUOLPhysics::PhysicsStaticActor>>> _physicsStaticActors;

		/**
		 * \brief 현재 씬 상의 Dynamic Actors
		 */
		std::unordered_map<DUOLCommon::tstring,
			std::pair<std::weak_ptr<DUOLGameEngine::Transform>, std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>>> _physicsDynamicActors;
#pragma endregion

	public:
        void Initialize();

        void UnInitialize();

	private:
		void InitializePhysicsCollider(const std::shared_ptr<DUOLGameEngine::ColliderBase>& collider) const;

		void InitializePhysicsGameObject(const std::shared_ptr<DUOLGameEngine::GameObject>& gameObject);

		/**
		 * \brief GameScene이 Awake할 때 기존 씬의 정보를 제거하고 다시 PhysX 씬을 구성합니다.
		 * \param gameObjectsInScene Awake 하는 Scene의 GameObjects
		 */
		void InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& gameObjectsInScene);

		/**
		 * \brief 디버그 정보를 Graphics Manager에게 그려달라고 전달합니다.
		 */
		void OnRender();

#pragma region PHYSICS_EVENTS
		/**
		 * \brief 물리를 시뮬레이션하고 결과를 게임 로직 전체에 적용합니다.
		 * \param deltaTime 프레임 간격 (scaled delta time)
		 */
		void Update(float deltaTime);

		/**
		 * \brief 물리 시뮬레이션 전, 현재 트랜스폼들의 위치, 회전을 Actor에 적용합니다.
		 */
		void ApplyPhysicsTransformBeforeSimulate();

		/**
		 * \brief 물리 시뮬레이션의 결과 (위치, 회전, ..) 을 게임 로직 트랜스폼에 적용합니다.
		 */
		void ApplyPhysicsSimulateResult();
		
		/**
		 * \brief 등록된 모든 FixeUpdate 이벤트 핸들러를 호출합니다.
		 */
		void FixedUpdate();

        DUOLCommon::Event<void, float> _fixedUpdateEventHandlers;

        DUOLCommon::EventHandlerID AddFixedUpdateEventHandler(std::function<void(float)> functor);

        bool RemoveFixedUpdateEventHandler(DUOLCommon::EventHandlerID id);

		/**
		 * \brief 등록된 모든 OnTriggerXXX 이벤트 핸들러를 호출합니다.
		 */
		void TriggerUpdate();

		/**
		 * \brief 등록된 모든 OnCollisionXXX 이벤트 핸들러를 호출합니다.
		 */
		void CollisionUpdate();
#pragma endregion

#pragma region FRIEND_CLASS
		friend class SceneManager;
		
        friend class MonoBehaviourBase;

		friend class Engine;
#pragma endregion
	};
}
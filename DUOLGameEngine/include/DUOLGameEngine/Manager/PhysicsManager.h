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
    class ColliderBase;
}

namespace DUOLGameEngine
{
	class PhysicsManager final : public SingletonBase<PhysicsManager>
	{
		DECLARE_SINGLETON(PhysicsManager)

		DELETE_COPY_MOVE(PhysicsManager)

	private:
        virtual ~PhysicsManager() override;

        float _fixedTimeStep;

		/**
		 * \brief PhysX Engine Wrapper.
		 */
		std::shared_ptr<DUOLPhysics::PhysicsSystem> _physicsSystem;

	public:
        void Initialize();

        void UnInitialize();

        void Update(float deltaTime);

	private:
#pragma region PHYSICS_EVENTS
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
        friend class MonoBehaviourBase;

		friend class Engine;
#pragma endregion
	};
}

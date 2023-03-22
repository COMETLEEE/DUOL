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
	/**
	 * \brief For physics interpolate.
	 */
	struct PhysicsInterpolateData
	{
		DUOLMath::Vector3 _position;

		DUOLMath::Quaternion _rotation;

		float _timePoint;
	};

	// TODO : 씬에서 새로운 게임 오브젝트가 생성되는데 만약 .. 물리 관련된 오브젝트라면
	// TODO : PhysicsManager에게 콜백 요청할 수 있도록 해야합니다.

	/**
	 * \brief 물리 시뮬레이션 담당하는 매니저입니다.
	 */
	class DUOL_GAMEENGINE_API PhysicsManager final : public SingletonBase<PhysicsManager>
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
		 * \brief 현재 시스템 상의 Shapes.
		 */
		std::unordered_map<DUOLCommon::tstring, std::weak_ptr<DUOLPhysics::PhysicsShapeBase>> _physicsShapes;

		/**
		 * \brief 현재 씬 상의 Static Actors
		 */
		std::unordered_map<DUOLCommon::tstring,
			std::pair<DUOLGameEngine::Transform*, std::weak_ptr<DUOLPhysics::PhysicsStaticActor>>> _physicsStaticActors;
		 
		/**
		 * \brief 현재 씬 상의 Dynamic Actors
		 */
		std::unordered_map<DUOLCommon::tstring,
			std::pair<DUOLGameEngine::Transform*, std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>>> _physicsDynamicActors;

		/**
		 * \brief Dynamic Actor => Interpolate 체크된 개체들의 정보를 담아둡니다.
		 */
		std::unordered_map<DUOLCommon::tstring,
		                   std::pair<DUOLGameEngine::Transform*, std::pair<PhysicsInterpolateData, PhysicsInterpolateData>>> _physicsInterpolateDatas;
#pragma endregion

	public:
        void Initialize();

        void UnInitialize();

	private:
		/**
		 * \brief 콜라이더를 등록하고 내부 정보를 채웁니다.
		 * \param collider 등록할 콜라이더
		 */
		void InitializePhysicsCollider(DUOLGameEngine::ColliderBase* collider);

		/**
		 * \brief 물리와 관련된 게임 오브젝트를 등록합니다.
		 * \param gameObject 등록할 게임 오브젝트
		 */
		void InitializePhysicsGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively = true);

		/**
		 * \brief 등록된 물리 관련 오브젝트를 physicsScene에서 해제합니다.
		 * \param gameObject 해제할 게임 오브젝트
		 */
		void UnInitializePhysicsGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively = true);

		/**
		 * \brief Game Scene이 Awake할 때 기존 씬의 정보를 제거하고 다시 PhysX 씬을 구성합니다.
		 * \param rootObjectsInScene Awake 하는 Scene의 GameObjects
		 */
		void InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene);

		/**
		 * \brief PhysX Dynamic Actor를 붙입니다.
		 * \param gameObject 해당 게임 오브젝트
		 */
		void AttachPhysicsDynamicActor(DUOLGameEngine::GameObject* gameObject, DUOLGameEngine::Rigidbody* rigidbody);

		/**
		 * \brief PhysX Static Actor를 붙입니다.
		 * \param gameObject 해당 게임 오브젝트
		 */
		void AttachPhysicsStaticActor(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief PhysX Collider를 붙입니다.
		 * \param gameObject 해당 게임 오브젝트
		 */
		void AttachPhysicsCollider(DUOLGameEngine::GameObject* gameObject, DUOLGameEngine::ColliderBase* collider);

		/**
		 * \brief 해당 게임 오브젝트에 달려 있는 PhysX Actor 를 제거합니다.
		 * \param gameObject 해당 게임 오브젝트
		 */
		void DetachPhysicsActor(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief PhysX Collider 를 제거합니다.
		 * \param gameObject 해당 게임 오브젝트
		 */
		void DetachPhysicsCollider(DUOLGameEngine::GameObject* gameObject, DUOLGameEngine::ColliderBase* collider);

#pragma region PHYSICS_EVENTS
	public:
		/**
		 * \brief 물리를 시뮬레이션하고 결과를 게임 로직 전체에 적용합니다.
		 * \param deltaTime 프레임 간격 (scaled delta time)
		 */
		void Update(float deltaTime);

		void ApplyPhysicsInterpolate();

		/**
		 * \brief 게임 로직에 적용하지는 않지만, 피직스 씬에 변동사항을 적용합니다. 에디트 또는 퍼즈 모드 전용입니다.
		 * \param deltaTime 프레임 간격 (scaled delta time)
		 */
		void UpdateEditAndPauseMode(float deltaTime);

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

        DUOLCommon::EventListenerID AddFixedUpdateEventHandler(std::function<void(float)> functor);

        bool RemoveFixedUpdateEventHandler(DUOLCommon::EventListenerID id);
#pragma endregion

	public:
		DUOLMath::Vector3 GetGravity();

#pragma region PHYSICS_UTIL
		bool Raycast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& end, DUOLPhysics::RaycastHit& outRaycastHit);

		bool Raycast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outRaycastHit);

		bool Spherecast(const DUOLMath::Vector3& start, const DUOLMath::Vector3& direction, float radius, float maxDistance, DUOLPhysics::RaycastHit& outSpherecastHit);
#pragma endregion

#pragma region FRIEND_CLASS
		friend class SceneManager;

		friend class DebugManager;

        friend class MonoBehaviourBase;

		friend class Engine;

		friend class Scene;

		friend class GameObject;

		friend class Rigidbody;

		friend class ColliderBase;
#pragma endregion
	};
}
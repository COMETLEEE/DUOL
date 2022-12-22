/**

    @file      Rigidbody.h
    @brief     Control of an object's position through physics simulation.
    @details   ~d
    @author    COMETLEE
    @date      15.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/ComponentBase.h"

#include "DUOLPhysics/Actor/PhysicsDynamicActor.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Dynamic physics 시뮬레이션을 제공합니다.
	 */
	class Rigidbody final : public DUOLGameEngine::ComponentBase
	{
	public:
		Rigidbody(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("RigidBody"));

		virtual ~Rigidbody() override;

	private:
		/**
		 * \brief PhysX Dynamic Actor (inherit Rigidbody in PhysX)
		 * => 원시 포인터로 캐싱해도 되지 않을까 ..?
		 */
		std::weak_ptr<DUOLPhysics::PhysicsDynamicActor> _dynamicActor;

		bool _useGravity;

		float _mass;

		bool _isFreezeRotation;

	private:
		void OnInitializeDynamicActor(const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>& dynamicActor);
		
	public:
		bool GetUseGravity() const;

		void SetUseGravity(bool value);

		void AddForce(const DUOLMath::Vector3& force) const;

		void AddTorque(const DUOLMath::Vector3& force) const;

		void AddImpulse(const DUOLMath::Vector3& force) const;

		void SetIsFreezeRotation(bool value);

		DUOLMath::Vector3 GetLinearVelocity() const;

		void SetLinearVelocity(const DUOLMath::Vector3& velocity);

		DUOLMath::Vector3 GetAngularVelocity() const;

		void SetAngularVelocity(const DUOLMath::Vector3& velocity);

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}
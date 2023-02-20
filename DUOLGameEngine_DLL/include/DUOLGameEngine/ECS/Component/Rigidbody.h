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
	class DUOL_GAMEENGINE_API Rigidbody final : public DUOLGameEngine::ComponentBase
	{
	public:
		Rigidbody(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Rigidbody"));

		virtual ~Rigidbody() override;

	private:
		/**
		 * \brief PhysX Dynamic Actor (inherit Rigidbody in PhysX)
		 * => 원시 포인터로 캐싱해도 되지 않을까 ..?
		 */
		std::weak_ptr<DUOLPhysics::PhysicsDynamicActor> _dynamicActor;

		bool _useGravity;

		float _mass;

		DUOLMath::Vector3 _centerOfMass;

		bool _isFreezeRotation;

		bool _isFreezePosition;

	private:
		void OnInitializeDynamicActor(const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>& dynamicActor);

		virtual void OnDestroy() override;

	public:
		bool GetUseGravity() const;

		void SetUseGravity(bool value);

		float GetMass();

		void SetMass(float value);

		void AddForce(const DUOLMath::Vector3& force) const;

		void AddTorque(const DUOLMath::Vector3& force) const;

		void AddImpulse(const DUOLMath::Vector3& force) const;

		bool GetIsFreezeRotation() const;

		void SetIsFreezeRotation(bool value);

		bool GetIsFreezePosition() const;

		void SetIsFreezePosition(bool value);

		DUOLMath::Vector3 GetLinearVelocity() const;

		void SetLinearVelocity(const DUOLMath::Vector3& velocity);

		DUOLMath::Vector3 GetAngularVelocity() const;

		void SetAngularVelocity(const DUOLMath::Vector3& velocity);

		const DUOLMath::Vector3& GetCenterOfMass() const;

		void SetCenterOfMass(const DUOLMath::Vector3& center);

		RTTR_ENABLE(DUOLGameEngine::ComponentBase)

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}
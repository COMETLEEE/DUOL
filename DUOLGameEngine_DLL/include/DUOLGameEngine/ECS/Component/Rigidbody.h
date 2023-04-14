/**

    @file      Rigidbody.h
    @brief     Control of an object's position through physics simulation.
    @details   ~d
    @author    COMETLEE
    @date      15.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <queue>

#include "Transform.h"
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

		std::vector<std::function<void(void)>> _allPhysicsMessages;
		
		bool _useGravity;

		float _mass;

		DUOLMath::Vector3 _centerOfMass;

		bool _isInterpolate;

		bool _isFreezeXRotation;

		bool _isFreezeYRotation;

		bool _isFreezeZRotation;

		bool _isFreezeXPosition;

		bool _isFreezeYPosition;

		bool _isFreezeZPosition;

		bool _isKinematic;

		DUOLPhysics::AxisLockFlags _axisLockFlags;

	private:
		void OnInitializeDynamicActor(const std::weak_ptr<DUOLPhysics::PhysicsDynamicActor>& dynamicActor);

		void ExecuteAllMessages();

		virtual void OnDestroy() override;

	public:
		bool GetUseGravity() const;

		void SetUseGravity(bool value);

		float GetMass();

		void SetMass(float value);

		void AddForce(const DUOLMath::Vector3& force);

		void AddTorque(const DUOLMath::Vector3& force);

		void AddImpulse(const DUOLMath::Vector3& force);

		bool GetIsFreezeXRotation() const;
		bool GetIsFreezeYRotation() const;
		bool GetIsFreezeZRotation() const;

		void SetIsFreezeXRotation(bool value);
		void SetIsFreezeYRotation(bool value);
		void SetIsFreezeZRotation(bool value);

		bool GetIsFreezeXPosition() const;
		bool GetIsFreezeYPosition() const;
		bool GetIsFreezeZPosition() const;

		void SetIsFreezeXPosition(bool value);
		void SetIsFreezeYPosition(bool value);
		void SetIsFreezeZPosition(bool value);

		DUOLMath::Vector3 GetLinearVelocity() const;

		void SetLinearVelocity(const DUOLMath::Vector3& velocity);

		DUOLMath::Vector3 GetAngularVelocity() const;

		void SetAngularVelocity(const DUOLMath::Vector3& velocity);

		const DUOLMath::Vector3& GetCenterOfMass() const;

		/**
		 * \brief Setter 들 추가되면 OnInitializeDynamicActor에 내용 넣어줍시다 ..!
		 * \param center 
		 */
		void SetCenterOfMass(const DUOLMath::Vector3& center);

		bool GetIsKinematic() const;

		void SetIsKinematic(bool value);

		bool GetIsInterpolate() const;

		void SetIsInterpolate(bool value);

		RTTR_ENABLE(DUOLGameEngine::ComponentBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}
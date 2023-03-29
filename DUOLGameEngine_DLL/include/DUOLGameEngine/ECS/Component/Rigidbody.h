/**

    @file      Rigidbody.h
    @brief     Control of an object's position through physics simulation.
    @details   ~d
    @author    COMETLEE
    @date      15.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
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

		virtual void OnDestroy() override;

	public:
		bool GetUseGravity() const;

		void SetUseGravity(bool value);

		float GetMass();

		void SetMass(float value);

		void AddForce(const DUOLMath::Vector3& force) const;

		void AddTorque(const DUOLMath::Vector3& force) const;

		void AddImpulse(const DUOLMath::Vector3& force) const;

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

		/**
		 * \brief 물리 엔진 세상의 로테이션 상태를 조정합니다.
		 * \param rotation 회전값
		 * \param relativeTo 어디를 기준으로
		 */
		void SetRotation(const DUOLMath::Quaternion& rotation, DUOLGameEngine::Space relativeTo = DUOLGameEngine::Space::World);

		RTTR_ENABLE(DUOLGameEngine::ComponentBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}
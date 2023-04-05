/**
	@file      Projectile.h
	@brief     투사체 클래스.플레이어, 적 상관 없이 공용으로 사용할 수 있는 투사체.
	@details   ~ 태그로 타겟을 판별한다.
	@author    SinSeongHyeon
	@date      04.05.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Rigidbody;
	class SphereCollider;
}

namespace DUOLClient
{
	class DUOL_CLIENT_API Projectile : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Projectile(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Projectile"));

		virtual ~Projectile() override = default;
	private:
		float _damage;

		float _speed;

		float _radius;

		DUOLGameEngine::SphereCollider* _collider;

		DUOLGameEngine::Rigidbody* _rigidbody;

		DUOLGameEngine::Transform* _transform;

		DUOLCommon::tstring _targetTag;

		DUOLMath::Vector3 _currentVelocity;

		bool _isGravity;

		DUOLGameEngine::GameObject* _projectileOwner;

	public:
		void FireProjectile(DUOLMath::Vector3 direction, float speed, DUOLGameEngine::GameObject* owner = nullptr, float damage = 0, DUOLCommon::tstring targetTag = TEXT(""), bool isGravity = true, float radius = 1.0f);

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};
}

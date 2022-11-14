/**

    @file      ColliderBase.h
    @brief     DUOLGameEngine 물리 충돌 관련 컴포넌트들의 기본 클래스
    @details   ~
    @author    COMETLEE
    @date      14.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	class Rigidbody;

	/**
	 * \brief 모든 물리 상호작용이 가능한 컴포넌트들의 기본 클래스
	 */
	class ColliderBase : public DUOLGameEngine::BehaviourBase
	{
	public:
		ColliderBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("ColliderBase"));

		virtual ~ColliderBase() override;

	private:
		/**
		 * \brief The Rigidbody the collider is attached to.
		 */
		std::weak_ptr<DUOLGameEngine::Rigidbody> _attachedRigidbody;

		/**
		 * \brief Trigger option 으로 충돌 연산을 진행합니까 ?
		 */
		bool _isTrigger;

	public:
		inline const std::shared_ptr<DUOLGameEngine::Rigidbody> GetAttachedRigidbody() { return _attachedRigidbody.lock(); }

		void SetAttachedRigidbody(const std::weak_ptr<DUOLGameEngine::Rigidbody>& rigidbody);

		inline bool GetIsTrigger() const { return _isTrigger; }

		void SetIsTrigger(bool value);
	};
}
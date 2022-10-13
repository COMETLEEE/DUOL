#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	struct Collision;

	class ColliderBase;

	/**
	 * \brief 스크립트 (== 커스텀 컴포넌트) 의 기본 클래스입니다.
	 * 스크립트는 해당 게임 오브젝트의 생애 동안 조작을 담당합니다.
	 */
	class MonoBehaviourBase : public BehaviourBase
	{
	public:
		MonoBehaviourBase(std::shared_ptr<DUOLGameEngine::GameObject> owner, const tstring& name = StringHelper::ToTString("MonoBehaviour"));

		virtual ~MonoBehaviourBase() override;

		DEFINE_DEFAULT_COPY_MOVE(MonoBehaviourBase)

		virtual void OnCollisionEnter(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionStay(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionExit(std::shared_ptr<Collision> collision) { }

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> collision) { }

		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) { }

		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) { }
	};
}
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	struct Collision;

	class ColliderBase;

	/**
	 * \brief ��ũ��Ʈ (== Ŀ���� ������Ʈ) �� �⺻ Ŭ�����Դϴ�.
	 * ��ũ��Ʈ�� �ش� ���� ������Ʈ�� ���� ���� ������ ����մϴ�.
	 */
	class MonoBehaviourBase : public BehaviourBase
	{
	public:
		virtual void OnCollisionEnter(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionStay(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionExit(std::shared_ptr<Collision> collision) { }

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> collision) { }

		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) { }

		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) { }
	};
}
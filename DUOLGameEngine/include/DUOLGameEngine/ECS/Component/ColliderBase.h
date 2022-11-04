#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief �浹 �� �浹 ������ ��� �ִ� Ŭ����
	 */
	struct Collision
	{
		
	};

	/**
	 * \brief ��� ���� ��ȣ�ۿ��� ������ ������Ʈ���� �⺻ Ŭ����
	 */
	class ColliderBase : public BehaviourBase
	{
	public:
		ColliderBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("ColliderBase"));

		virtual ~ColliderBase();
	};
}
#pragma once
#include <memory>

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief Ȱ��ȭ, ��Ȱ��ȭ�� ������ �� �ִ� ������Ʈ�Դϴ�.
	 */
	class BehaviourBase : public DUOLGameEngine::ComponentBase
	{
	private:
		BehaviourBase(std::shared_ptr<GameObject> owner);

		bool _isEnabled;

		bool _isActivedAndEnabled;
	};
}
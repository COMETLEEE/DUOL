#pragma once
#include <memory>

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief 활성화, 비활성화를 선택할 수 있는 컴포넌트입니다.
	 */
	class BehaviourBase : public DUOLGameEngine::ComponentBase
	{
	private:
		BehaviourBase(std::shared_ptr<GameObject> owner);

		bool _isEnabled;

		bool _isActivedAndEnabled;
	};
}
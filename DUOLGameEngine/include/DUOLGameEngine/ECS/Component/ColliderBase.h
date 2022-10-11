#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 충돌 시 충돌 정보를 담고 있는 클래스
	 */
	struct Collision
	{
		
	};

	/**
	 * \brief 모든 물리 상호작용이 가능한 컴포넌트들의 기본 클래스
	 */
	class ColliderBase : public BehaviourBase
	{
	};
}